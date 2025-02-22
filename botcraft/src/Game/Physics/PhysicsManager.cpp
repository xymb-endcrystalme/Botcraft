#include "botcraft/Game/Physics/PhysicsManager.hpp"
#include "botcraft/Utilities/Logger.hpp"
#include "botcraft/Utilities/SleepUtilities.hpp"
#include "botcraft/Game/Entities/EntityManager.hpp"
#include "botcraft/Game/Entities/LocalPlayer.hpp"
#include "botcraft/Network/NetworkManager.hpp"
#include "botcraft/Game/World/World.hpp"
#if USE_GUI
#include "botcraft/Renderer/RenderingManager.hpp"
#endif

using namespace ProtocolCraft;

namespace Botcraft
{
    PhysicsManager::PhysicsManager(
#if USE_GUI
        const std::shared_ptr<Renderer::RenderingManager>& rendering_manager_,
#endif   
        const std::shared_ptr<EntityManager>& entity_manager_,
        const std::shared_ptr<World>& world_,
        const std::shared_ptr<NetworkManager>& network_manager_)
    {
#if USE_GUI
        rendering_manager = rendering_manager_;
#endif   
        entity_manager = entity_manager_;
        world = world_;
        network_manager = network_manager_;
    }

    PhysicsManager::~PhysicsManager()
    {
        StopPhysics();
    }

    void PhysicsManager::StartPhysics()
    {
        should_run = true;

        // Launch the physics thread (continuously sending the position to the server)
        thread_physics = std::thread(&PhysicsManager::RunSyncPos, this);
    }

    void PhysicsManager::StopPhysics()
    {
        should_run = false;
        if (thread_physics.joinable())
        {
            thread_physics.join();
        }
    }

    void PhysicsManager::SetShouldFallInVoid(const bool b)
    {
        should_fall_in_void = b;
    }



    void PhysicsManager::RunSyncPos()
    {
        Logger::GetInstance().RegisterThread("RunSyncPos");

        // Wait for 500 milliseconds before starting to send position continuously
        // TODO: wait for something better?
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        auto last_send = std::chrono::steady_clock::now();
        std::shared_ptr<ServerboundMovePlayerPacketPosRot> msg_position = std::make_shared<ServerboundMovePlayerPacketPosRot>();
        bool has_moved = false;

        while (should_run)
        {
            // End of the current tick
            auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);

            if (network_manager->GetConnectionState() == ProtocolCraft::ConnectionState::Play)
            {
                std::shared_ptr<LocalPlayer> local_player = entity_manager->GetLocalPlayer();
                if (local_player && local_player->GetPosition().y < 1000.0)
                {
                    bool is_loaded = false;
                    bool is_in_fluid = false;
                    std::lock_guard<std::mutex> player_guard(local_player->GetMutex());
                    {
                        std::lock_guard<std::mutex> mutex_guard(world->GetMutex());
                        const Position player_position = Position(std::floor(local_player->GetX()), std::floor(local_player->GetY()), std::floor(local_player->GetZ()));

                        is_loaded = world->IsLoaded(player_position);

                        if (is_loaded)
                        {
                            const Block* block_ptr = world->GetBlock(player_position);
                            is_in_fluid = block_ptr && block_ptr->GetBlockstate()->IsFluid();
                        }
                    }

                    if (is_loaded)
                    {
                        //Check that we did not go through a block
                        Physics(is_in_fluid);

                        if (local_player->GetHasMoved() ||
                            std::abs(local_player->GetSpeed().x) > 1e-3 ||
                            std::abs(local_player->GetSpeed().y) > 1e-3 ||
                            std::abs(local_player->GetSpeed().z) > 1e-3)
                        {
                            has_moved = true;
                            // Reset the player move state until next tick
                            local_player->SetHasMoved(false);
                        }
                        else
                        {
                            has_moved = false;
                        }

                        //Avoid forever falling if position is in the void
                        if (!should_fall_in_void && local_player->GetPosition().y <= world->GetMinY())
                        {
                            local_player->SetY(world->GetMinY());
                            local_player->SetSpeedY(0.0);
                            local_player->SetOnGround(true);
                        }

                        // Reset the speed until next frame
                        // Update the gravity value if needed
                        local_player->SetSpeedX(0.0);
                        local_player->SetSpeedZ(0.0);
                        if (local_player->GetOnGround())
                        {
                            local_player->SetSpeedY(0.0);
                        }
                        else
                        {
                            local_player->SetSpeedY((local_player->GetSpeed().y - 0.08) * 0.98);//TODO replace hardcoded value?
                        }
                    }

#if USE_GUI
                    if (rendering_manager && has_moved)
                    {
                        rendering_manager->SetPosOrientation(local_player->GetPosition().x, local_player->GetPosition().y + 1.62, local_player->GetPosition().z, local_player->GetYaw(), local_player->GetPitch());
                    }
#endif
                    if (has_moved || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_send).count() >= 1000)
                    {
                        msg_position->SetX(local_player->GetPosition().x);
                        msg_position->SetY(local_player->GetPosition().y);
                        msg_position->SetZ(local_player->GetPosition().z);
                        msg_position->SetYRot(local_player->GetYaw());
                        msg_position->SetXRot(local_player->GetPitch());
                        msg_position->SetOnGround(local_player->GetOnGround());

                        network_manager->Send(msg_position);
                        last_send = std::chrono::steady_clock::now();
                    }
                }
            }
            SleepUntil(end);
        }
    }

    void PhysicsManager::Physics(const bool is_in_fluid)
    {
        if (!entity_manager)
        {
            return;
        }

        std::shared_ptr<LocalPlayer> local_player = entity_manager->GetLocalPlayer();

        //If the player did not move we assume it does not collide
        /*if (!local_player->GetHasMoved() &&
            abs(local_player->GetSpeed().x) < 1e-3 &&
            abs(local_player->GetSpeed().y) < 1e-3 &&
            abs(local_player->GetSpeed().z) < 1e-3)
        {
            // We could return if we were sure that we have a block under our feet
            return;
        }*/

        // Player mutex is already locked by calling function
        Position player_position(std::floor(local_player->GetX()), std::floor(local_player->GetY()), std::floor(local_player->GetX()));
        Vector3<double> min_player_collider, max_player_collider;
        for (int i = 0; i < 3; ++i)
        {
            min_player_collider[i] = std::min(local_player->GetCollider().GetMin()[i], local_player->GetCollider().GetMin()[i] + local_player->GetSpeed()[i]);
            max_player_collider[i] = std::max(local_player->GetCollider().GetMax()[i], local_player->GetCollider().GetMax()[i] + local_player->GetSpeed()[i]);
        }

        AABB broadphase_collider = AABB((min_player_collider + max_player_collider) / 2.0, (max_player_collider - min_player_collider) / 2.0);

        bool has_hit_down = false;
        bool has_hit_up = false;

        Position cube_pos;
        for (int x = (int)std::floor(min_player_collider.x); x < (int)std::ceil(max_player_collider.x); ++x)
        {
            cube_pos.x = x;
            for (int y = (int)std::floor(min_player_collider.y); y < (int)std::ceil(max_player_collider.y); ++y)
            {
                cube_pos.y = y;
                for (int z = (int)std::floor(min_player_collider.z); z < (int)std::ceil(max_player_collider.z); ++z)
                {
                    cube_pos.z = z;

                    Block block;
                    {
                        std::lock_guard<std::mutex> mutex_guard(world->GetMutex());
                        const Block* block_ptr = world->GetBlock(cube_pos);

                        if (block_ptr == nullptr)
                        {
                            continue;
                        }
                        block = *block_ptr;
                    }

                    if (!is_in_fluid && !block.GetBlockstate()->IsSolid())
                    {
                        continue;
                    }

                    if (is_in_fluid &&
                        !block.GetBlockstate()->IsSolid() &&
                        (!block.GetBlockstate()->IsFluid() ||
                            cube_pos.y >= player_position.y))
                    {
                        continue;
                    }

                    const std::vector<AABB>& block_colliders = block.GetBlockstate()->GetModel(block.GetModelId()).GetColliders();

                    for (int i = 0; i < block_colliders.size(); ++i)
                    {
                        if (!broadphase_collider.Collide(block_colliders[i] + Vector3<double>(cube_pos.x, cube_pos.y, cube_pos.z)))
                        {
                            continue;
                        }

                        Vector3<double> normal;
                        const double speed_fraction = local_player->GetCollider().SweptCollide(local_player->GetSpeed(), block_colliders[i] + Vector3<double>(cube_pos.x, cube_pos.y, cube_pos.z), normal);

                        if (speed_fraction < 1.0)
                        {
                            const Vector3<double> remaining_speed = local_player->GetSpeed() * (1.0 - speed_fraction);

                            // We remove epsilon to be sure we do not go
                            // through the face due to numerical imprecision
                            local_player->SetSpeed(local_player->GetSpeed() * (speed_fraction - 1e-6) + // Base speed truncated
                                (remaining_speed - normal * remaining_speed.dot(normal))); // Remaining speed projected on the plane
                        }

                        if (normal.y == 1.0)
                        {
                            has_hit_down = true;
                        }
                        else if (normal.y == -1.0)
                        {
                            has_hit_up = true;
                        }

                    }
                }
            }
        }
        local_player->SetPosition(local_player->GetPosition() + local_player->GetSpeed());
        local_player->SetOnGround(has_hit_down);
        if (has_hit_up)
        {
            local_player->SetSpeedY(0.0);
        }
    }

} //Botcraft
