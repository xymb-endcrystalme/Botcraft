#pragma once

#include <memory>
#include <map>
#include <optional>
#include <any>

#include <nlohmann/json.hpp>

#if PROTOCOL_VERSION > 340
#include "protocolCraft/Types/Chat.hpp"
#endif
#include "protocolCraft/Types/Slot.hpp"

#include "botcraft/Game/AABB.hpp"
#include "botcraft/Game/Enums.hpp"
#include "botcraft/Game/Vector3.hpp"

#if USE_GUI
#include "botcraft/Game/Model.hpp"
#endif

namespace Botcraft
{
    enum class EntityType;
#if PROTOCOL_VERSION < 458
    enum class ObjectEntityType;
#endif

    class Entity
    {
    protected:
#if PROTOCOL_VERSION > 754
        static constexpr int metadata_count = 8;
#elif PROTOCOL_VERSION > 404
        static constexpr int metadata_count = 7;
#else
        static constexpr int metadata_count = 6;
#endif
        static const std::array<std::string, metadata_count> metadata_names;
        static constexpr int hierarchy_metadata_count = 0;

    public:
        Entity();
        virtual ~Entity();

        // Object related stuff
        virtual std::string GetName() const = 0;
        virtual EntityType GetType() const = 0;
        AABB GetCollider() const;
        virtual double GetWidth() const;
        virtual double GetHeight() const;

        // Metadata stuff
        void LoadMetadataFromRawArray(const std::vector<unsigned char>& data);
        virtual void SetMetadataValue(const int index, const std::any& value);

        char GetDataSharedFlagsId() const;
        int GetDataAirSupplyId() const;
#if PROTOCOL_VERSION > 340
        const std::optional<ProtocolCraft::Chat>& GetDataCustomName() const;
#else
        const std::string& GetDataCustomName() const;
#endif
        bool GetDataCustomNameVisible() const;
        bool GetDataSilent() const;
        bool GetDataNoGravity() const;
#if PROTOCOL_VERSION > 404
        Pose GetDataPose() const;
#endif
#if PROTOCOL_VERSION > 754
        int GetDataTicksFrozen() const;
#endif

        void SetDataSharedFlagsId(const char data_shared_flags_id);
        void SetDataAirSupplyId(const int data_air_supply_id);
#if PROTOCOL_VERSION > 340
        void SetDataCustomName(const std::optional<ProtocolCraft::Chat>& data_custom_name);
#else
        void SetDataCustomName(const std::string& data_custom_name);
#endif
        void SetDataCustomNameVisible(const bool data_custom_name_visible);
        void SetDataSilent(const bool data_silent);
        void SetDataNoGravity(const bool data_no_gravity);
#if PROTOCOL_VERSION > 404
        void SetDataPose(const Pose data_pose);
#endif
#if PROTOCOL_VERSION > 754
        void SetDataTicksFrozen(const int data_ticks_frozen);
#endif

        // Generic properties getter
        int GetEntityID() const;
        const Vector3<double>& GetPosition() const;
        double GetX() const;
        double GetY() const;
        double GetZ() const;
        float GetYaw() const;
        float GetPitch() const;
        const Vector3<double>& GetSpeed() const;
        double GetSpeedX() const;
        double GetSpeedY() const;
        double GetSpeedZ() const;
        bool GetOnGround() const;
        const std::map<EquipmentSlot, ProtocolCraft::Slot>& GetEquipments() const;
        const ProtocolCraft::Slot& GetEquipment(const EquipmentSlot slot) const;
#if USE_GUI
        const std::vector<Renderer::Face>& GetFaces();
        bool GetAreRenderedFacesUpToDate() const;
#endif

        // Generic properties setter
        void SetEntityID(const int entity_id_);
        virtual void SetPosition(const Vector3<double>& position_);
        virtual void SetX(const double x_);
        virtual void SetY(const double y_);
        virtual void SetZ(const double z_);
        virtual void SetYaw(const float yaw_);
        virtual void SetPitch(const float pitch_);
        void SetSpeed(const Vector3<double>& speed_);
        void SetSpeedX(const double speed_x_);
        void SetSpeedY(const double speed_y_);
        void SetSpeedZ(const double speed_z_);
        void SetOnGround(const bool on_ground_);
        void SetEquipment(const EquipmentSlot slot, const ProtocolCraft::Slot& item);
#if USE_GUI
        void SetAreRenderedFacesUpToDate(const bool are_rendered_faces_up_to_date_);
#endif

        // In case it's needed one day, could be useful
        virtual nlohmann::json Serialize() const;

        virtual bool IsLocalPlayer() const;
        // Can be used to know if an entity has a certain virtual type as ancestor
        virtual bool IsLivingEntity() const;
        virtual bool IsAbstractArrow() const;
        virtual bool IsAnimal() const;
        virtual bool IsAmbientCreature() const;
        virtual bool IsMonster() const;
        virtual bool IsTamableAnimal() const;
        virtual bool IsAbstractSchoolingFish() const;
        virtual bool IsWaterAnimal() const;
        virtual bool IsAbstractChestedHorse() const;
        virtual bool IsAbstractHurtingProjectile() const;
        virtual bool IsMob() const;
        virtual bool IsSpellcasterIllager() const;
#if PROTOCOL_VERSION > 578
        virtual bool IsProjectile() const;
#endif
        virtual bool IsFlyingMob() const;
        virtual bool IsAbstractHorse() const;
        virtual bool IsAbstractGolem() const;
        virtual bool IsHangingEntity() const;
        virtual bool IsFireball() const;
        virtual bool IsAbstractMinecart() const;
        virtual bool IsAbstractMinecartContainer() const;
        virtual bool IsShoulderRidingEntity() const;
#if PROTOCOL_VERSION > 736
        virtual bool IsAbstractPiglin() const;
#endif
        virtual bool IsAbstractIllager() const;
        virtual bool IsAbstractFish() const;
#if PROTOCOL_VERSION > 404
        virtual bool IsRaider() const;
#endif
        virtual bool IsAbstractSkeleton() const;
        virtual bool IsThrowableItemProjectile() const;
#if PROTOCOL_VERSION > 477
        virtual bool IsAbstractVillager() const;
#endif
        virtual bool IsAgeableMob() const;
        virtual bool IsPathfinderMob() const;
#if PROTOCOL_VERSION > 404
        virtual bool IsPatrollingMonster() const;
#endif
        virtual bool IsThrowableProjectile() const;

        // Factory stuff
        static std::shared_ptr<Entity> CreateEntity(const EntityType type);
#if PROTOCOL_VERSION < 458
        static std::shared_ptr<Entity> CreateObjectEntity(const ObjectEntityType type);
#endif
    
    protected:
#if USE_GUI
        virtual void InitializeFaces();
#endif

    protected:
        int entity_id;
        Vector3<double> position;
        float yaw;
        float pitch;
        Vector3<double> speed;
        bool on_ground;
        std::map<EquipmentSlot, ProtocolCraft::Slot> equipments;

        std::map<std::string, std::any> metadata;

#if USE_GUI
        //All the faces of this model
        std::vector<FaceDescriptor> face_descriptors;
        std::vector<Renderer::Face> faces;

        bool are_rendered_faces_up_to_date;
#endif
    };

    enum class EntityType
    {
        None = -1,
#if PROTOCOL_VERSION > 754 // 1.17+
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Axolotl = 3,
        Bat = 4,
        Bee = 5,
        Blaze = 6,
        Boat = 7,
        Cat = 8,
        CaveSpider = 9,
        Chicken = 10,
        Cod = 11,
        Cow = 12,
        Creeper = 13,
        Dolphin = 14,
        Donkey = 15,
        DragonFireball = 16,
        Drowned = 17,
        ElderGuardian = 18,
        EndCrystal = 19,
        EnderDragon = 20,
        EnderMan = 21,
        Endermite = 22,
        Evoker = 23,
        EvokerFangs = 24,
        ExperienceOrb = 25,
        EyeOfEnder = 26,
        FallingBlockEntity = 27,
        FireworkRocketEntity = 28,
        Fox = 29,
        Ghast = 30,
        Giant = 31,
        GlowItemFrame = 32,
        GlowSquid = 33,
        Goat = 34,
        Guardian = 35,
        Hoglin = 36,
        Horse = 37,
        Husk = 38,
        Illusioner = 39,
        IronGolem = 40,
        ItemEntity = 41,
        ItemFrame = 42,
        LargeFireball = 43,
        LeashFenceKnotEntity = 44,
        LightningBolt = 45,
        Llama = 46,
        LlamaSpit = 47,
        MagmaCube = 48,
        Marker = 49,
        Minecart = 50,
        MinecartChest = 51,
        MinecartCommandBlock = 52,
        MinecartFurnace = 53,
        MinecartHopper = 54,
        MinecartSpawner = 55,
        MinecartTNT = 56,
        Mule = 57,
        MushroomCow = 58,
        Ocelot = 59,
        Painting = 60,
        Panda = 61,
        Parrot = 62,
        Phantom = 63,
        Pig = 64,
        Piglin = 65,
        PiglinBrute = 66,
        Pillager = 67,
        PolarBear = 68,
        PrimedTnt = 69,
        Pufferfish = 70,
        Rabbit = 71,
        Ravager = 72,
        Salmon = 73,
        Sheep = 74,
        Shulker = 75,
        ShulkerBullet = 76,
        Silverfish = 77,
        Skeleton = 78,
        SkeletonHorse = 79,
        Slime = 80,
        SmallFireball = 81,
        SnowGolem = 82,
        Snowball = 83,
        SpectralArrow = 84,
        Spider = 85,
        Squid = 86,
        Stray = 87,
        Strider = 88,
        ThrownEgg = 89,
        ThrownEnderpearl = 90,
        ThrownExperienceBottle = 91,
        ThrownPotion = 92,
        ThrownTrident = 93,
        TraderLlama = 94,
        TropicalFish = 95,
        Turtle = 96,
        Vex = 97,
        Villager = 98,
        Vindicator = 99,
        WanderingTrader = 100,
        Witch = 101,
        WitherBoss = 102,
        WitherSkeleton = 103,
        WitherSkull = 104,
        Wolf = 105,
        Zoglin = 106,
        Zombie = 107,
        ZombieHorse = 108,
        ZombieVillager = 109,
        ZombifiedPiglin = 110,
        Player = 111,
        FishingHook = 112,
#elif PROTOCOL_VERSION > 736 // 1.16.2+
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Bee = 4,
        Blaze = 5,
        Boat = 6,
        Cat = 7,
        CaveSpider = 8,
        Chicken = 9,
        Cod = 10,
        Cow = 11,
        Creeper = 12,
        Dolphin = 13,
        Donkey = 14,
        DragonFireball = 15,
        Drowned = 16,
        ElderGuardian = 17,
        EndCrystal = 18,
        EnderDragon = 19,
        EnderMan = 20,
        Endermite = 21,
        Evoker = 22,
        EvokerFangs = 23,
        ExperienceOrb = 24,
        EyeOfEnder = 25,
        FallingBlockEntity = 26,
        FireworkRocketEntity = 27,
        Fox = 28,
        Ghast = 29,
        Giant = 30,
        Guardian = 31,
        Hoglin = 32,
        Horse = 33,
        Husk = 34,
        Illusioner = 35,
        IronGolem = 36,
        ItemEntity = 37,
        ItemFrame = 38,
        LargeFireball = 39,
        LeashFenceKnotEntity = 40,
        LightningBolt = 41,
        Llama = 42,
        LlamaSpit = 43,
        MagmaCube = 44,
        Minecart = 45,
        MinecartChest = 46,
        MinecartCommandBlock = 47,
        MinecartFurnace = 48,
        MinecartHopper = 49,
        MinecartSpawner = 50,
        MinecartTNT = 51,
        Mule = 52,
        MushroomCow = 53,
        Ocelot = 54,
        Painting = 55,
        Panda = 56,
        Parrot = 57,
        Phantom = 58,
        Pig = 59,
        Piglin = 60,
        PiglinBrute = 61,
        Pillager = 62,
        PolarBear = 63,
        PrimedTnt = 64,
        Pufferfish = 65,
        Rabbit = 66,
        Ravager = 67,
        Salmon = 68,
        Sheep = 69,
        Shulker = 70,
        ShulkerBullet = 71,
        Silverfish = 72,
        Skeleton = 73,
        SkeletonHorse = 74,
        Slime = 75,
        SmallFireball = 76,
        SnowGolem = 77,
        Snowball = 78,
        SpectralArrow = 79,
        Spider = 80,
        Squid = 81,
        Stray = 82,
        Strider = 83,
        ThrownEgg = 84,
        ThrownEnderpearl = 85,
        ThrownExperienceBottle = 86,
        ThrownPotion = 87,
        ThrownTrident = 88,
        TraderLlama = 89,
        TropicalFish = 90,
        Turtle = 91,
        Vex = 92,
        Villager = 93,
        Vindicator = 94,
        WanderingTrader = 95,
        Witch = 96,
        WitherBoss = 97,
        WitherSkeleton = 98,
        WitherSkull = 99,
        Wolf = 100,
        Zoglin = 101,
        Zombie = 102,
        ZombieHorse = 103,
        ZombieVillager = 104,
        ZombifiedPiglin = 105,
        Player = 106,
        FishingHook = 107,
#elif PROTOCOL_VERSION > 578 // 1.16+
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Bee = 4,
        Blaze = 5,
        Boat = 6,
        Cat = 7,
        CaveSpider = 8,
        Chicken = 9,
        Cod = 10,
        Cow = 11,
        Creeper = 12,
        Dolphin = 13,
        Donkey = 14,
        DragonFireball = 15,
        Drowned = 16,
        ElderGuardian = 17,
        EndCrystal = 18,
        EnderDragon = 19,
        EnderMan = 20,
        Endermite = 21,
        Evoker = 22,
        EvokerFangs = 23,
        ExperienceOrb = 24,
        EyeOfEnder = 25,
        FallingBlockEntity = 26,
        FireworkRocketEntity = 27,
        Fox = 28,
        Ghast = 29,
        Giant = 30,
        Guardian = 31,
        Hoglin = 32,
        Horse = 33,
        Husk = 34,
        Illusioner = 35,
        IronGolem = 36,
        ItemEntity = 37,
        ItemFrame = 38,
        LargeFireball = 39,
        LeashFenceKnotEntity = 40,
        LightningBolt = 41,
        Llama = 42,
        LlamaSpit = 43,
        MagmaCube = 44,
        Minecart = 45,
        MinecartChest = 46,
        MinecartCommandBlock = 47,
        MinecartFurnace = 48,
        MinecartHopper = 49,
        MinecartSpawner = 50,
        MinecartTNT = 51,
        Mule = 52,
        MushroomCow = 53,
        Ocelot = 54,
        Painting = 55,
        Panda = 56,
        Parrot = 57,
        Phantom = 58,
        Pig = 59,
        Piglin = 60,
        Pillager = 61,
        PolarBear = 62,
        PrimedTnt = 63,
        Pufferfish = 64,
        Rabbit = 65,
        Ravager = 66,
        Salmon = 67,
        Sheep = 68,
        Shulker = 69,
        ShulkerBullet = 70,
        Silverfish = 71,
        Skeleton = 72,
        SkeletonHorse = 73,
        Slime = 74,
        SmallFireball = 75,
        SnowGolem = 76,
        Snowball = 77,
        SpectralArrow = 78,
        Spider = 79,
        Squid = 80,
        Stray = 81,
        Strider = 82,
        ThrownEgg = 83,
        ThrownEnderpearl = 84,
        ThrownExperienceBottle = 85,
        ThrownPotion = 86,
        ThrownTrident = 87,
        TraderLlama = 88,
        TropicalFish = 89,
        Turtle = 90,
        Vex = 91,
        Villager = 92,
        Vindicator = 93,
        WanderingTrader = 94,
        Witch = 95,
        WitherBoss = 96,
        WitherSkeleton = 97,
        WitherSkull = 98,
        Wolf = 99,
        Zoglin = 100,
        Zombie = 101,
        ZombieHorse = 102,
        ZombieVillager = 103,
        ZombifiedPiglin = 104,
        Player = 105,
        FishingHook = 106,
#elif PROTOCOL_VERSION > 498 // 1.15+
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Bee = 4,
        Blaze = 5,
        Boat = 6,
        Cat = 7,
        CaveSpider = 8,
        Chicken = 9,
        Cod = 10,
        Cow = 11,
        Creeper = 12,
        Donkey = 13,
        Dolphin = 14,
        DragonFireball = 15,
        Drowned = 16,
        ElderGuardian = 17,
        EndCrystal = 18,
        EnderDragon = 19,
        EnderMan = 20,
        Endermite = 21,
        EvokerFangs = 22,
        Evoker = 23,
        ExperienceOrb = 24,
        EyeOfEnder = 25,
        FallingBlockEntity = 26,
        FireworkRocketEntity = 27,
        Fox = 28,
        Ghast = 29,
        Giant = 30,
        Guardian = 31,
        Horse = 32,
        Husk = 33,
        Illusioner = 34,
        ItemEntity = 35,
        ItemFrame = 36,
        LargeFireball = 37,
        LeashFenceKnotEntity = 38,
        Llama = 39,
        LlamaSpit = 40,
        MagmaCube = 41,
        Minecart = 42,
        MinecartChest = 43,
        MinecartCommandBlock = 44,
        MinecartFurnace = 45,
        MinecartHopper = 46,
        MinecartSpawner = 47,
        MinecartTNT = 48,
        Mule = 49,
        MushroomCow = 50,
        Ocelot = 51,
        Painting = 52,
        Panda = 53,
        Parrot = 54,
        Pig = 55,
        Pufferfish = 56,
        PigZombie = 57,
        PolarBear = 58,
        PrimedTnt = 59,
        Rabbit = 60,
        Salmon = 61,
        Sheep = 62,
        Shulker = 63,
        ShulkerBullet = 64,
        Silverfish = 65,
        Skeleton = 66,
        SkeletonHorse = 67,
        Slime = 68,
        SmallFireball = 69,
        SnowGolem = 70,
        Snowball = 71,
        SpectralArrow = 72,
        Spider = 73,
        Squid = 74,
        Stray = 75,
        TraderLlama = 76,
        TropicalFish = 77,
        Turtle = 78,
        ThrownEgg = 79,
        ThrownEnderpearl = 80,
        ThrownExperienceBottle = 81,
        ThrownPotion = 82,
        ThrownTrident = 83,
        Vex = 84,
        Villager = 85,
        IronGolem = 86,
        Vindicator = 87,
        Pillager = 88,
        WanderingTrader = 89,
        Witch = 90,
        WitherBoss = 91,
        WitherSkeleton = 92,
        WitherSkull = 93,
        Wolf = 94,
        Zombie = 95,
        ZombieHorse = 96,
        ZombieVillager = 97,
        Phantom = 98,
        Ravager = 99,
        LightningBolt = 100,
        Player = 101,
        FishingHook = 102,
#elif PROTOCOL_VERSION > 404 //1.14+
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Blaze = 4,
        Boat = 5,
        Cat = 6,
        CaveSpider = 7,
        Chicken = 8,
        Cod = 9,
        Cow = 10,
        Creeper = 11,
        Donkey = 12,
        Dolphin = 13,
        DragonFireball = 14,
        Drowned = 15,
        ElderGuardian = 16,
        EndCrystal = 17,
        EnderDragon = 18,
        EnderMan = 19,
        Endermite = 20,
        EvokerFangs = 21,
        Evoker = 22,
        ExperienceOrb = 23,
        EyeOfEnder = 24,
        FallingBlockEntity = 25,
        FireworkRocketEntity = 26,
        Fox = 27,
        Ghast = 28,
        Giant = 29,
        Guardian = 30,
        Horse = 31,
        Husk = 32,
        Illusioner = 33,
        ItemEntity = 34,
        ItemFrame = 35,
        LargeFireball = 36,
        LeashFenceKnotEntity = 37,
        Llama = 38,
        LlamaSpit = 39,
        MagmaCube = 40,
        Minecart = 41,
        MinecartChest = 42,
        MinecartCommandBlock = 43,
        MinecartFurnace = 44,
        MinecartHopper = 45,
        MinecartSpawner = 46,
        MinecartTNT = 47,
        Mule = 48,
        MushroomCow = 49,
        Ocelot = 50,
        Painting = 51,
        Panda = 52,
        Parrot = 53,
        Pig = 54,
        Pufferfish = 55,
        PigZombie = 56,
        PolarBear = 57,
        PrimedTnt = 58,
        Rabbit = 59,
        Salmon = 60,
        Sheep = 61,
        Shulker = 62,
        ShulkerBullet = 63,
        Silverfish = 64,
        Skeleton = 65,
        SkeletonHorse = 66,
        Slime = 67,
        SmallFireball = 68,
        SnowGolem = 69,
        Snowball = 70,
        SpectralArrow = 71,
        Spider = 72,
        Squid = 73,
        Stray = 74,
        TraderLlama = 75,
        TropicalFish = 76,
        Turtle = 77,
        ThrownEgg = 78,
        ThrownEnderpearl = 79,
        ThrownExperienceBottle = 80,
        ThrownPotion = 81,
        ThrownTrident = 82,
        Vex = 83,
        Villager = 84,
        IronGolem = 85,
        Vindicator = 86,
        Pillager = 87,
        WanderingTrader = 88,
        Witch = 89,
        WitherBoss = 90,
        WitherSkeleton = 91,
        WitherSkull = 92,
        Wolf = 93,
        Zombie = 94,
        ZombieHorse = 95,
        ZombieVillager = 96,
        Phantom = 97,
        Ravager = 98,
        LightningBolt = 99,
        Player = 100,
        FishingHook = 101,
#elif PROTOCOL_VERSION > 340 // 1.13+
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Blaze = 4,
        Boat = 5,
        CaveSpider = 6,
        Chicken = 7,
        Cod = 8,
        Cow = 9,
        Creeper = 10,
        Donkey = 11,
        Dolphin = 12,
        DragonFireball = 13,
        Drowned = 14,
        ElderGuardian = 15,
        EndCrystal = 16,
        EnderDragon = 17,
        EnderMan = 18,
        Endermite = 19,
        EvokerFangs = 20,
        Evoker = 21,
        ExperienceOrb = 22,
        EyeOfEnder = 23,
        FallingBlockEntity = 24,
        FireworkRocketEntity = 25,
        Ghast = 26,
        Giant = 27,
        Guardian = 28,
        Horse = 29,
        Husk = 30,
        Illusioner = 31,
        ItemEntity = 32,
        ItemFrame = 33,
        LargeFireball = 34,
        LeashFenceKnotEntity = 35,
        Llama = 36,
        LlamaSpit = 37,
        MagmaCube = 38,
        Minecart = 39,
        MinecartChest = 40,
        MinecartCommandBlock = 41,
        MinecartFurnace = 42,
        MinecartHopper = 43,
        MinecartSpawner = 44,
        MinecartTNT = 45,
        Mule = 46,
        MushroomCow = 47,
        Ocelot = 48,
        Painting = 49,
        Parrot = 50,
        Pig = 51,
        Pufferfish = 52,
        PigZombie = 53,
        PolarBear = 54,
        PrimedTnt = 55,
        Rabbit = 56,
        Salmon = 57,
        Sheep = 58,
        Shulker = 59,
        ShulkerBullet = 60,
        Silverfish = 61,
        Skeleton = 62,
        SkeletonHorse = 63,
        Slime = 64,
        SmallFireball = 65,
        SnowGolem = 66,
        Snowball = 67,
        SpectralArrow = 68,
        Spider = 69,
        Squid = 70,
        Stray = 71,
        TropicalFish = 72,
        Turtle = 73,
        ThrownEgg = 74,
        ThrownEnderpearl = 75,
        ThrownExperienceBottle = 76,
        ThrownPotion = 77,
        Vex = 78,
        Villager = 79,
        IronGolem = 80,
        Vindicator = 81,
        Witch = 82,
        WitherBoss = 83,
        WitherSkeleton = 84,
        WitherSkull = 85,
        Wolf = 86,
        Zombie = 87,
        ZombieHorse = 88,
        ZombieVillager = 89,
        Phantom = 90,
        LightningBolt = 91,
        Player = 92,
        FishingHook = 93,
        ThrownTrident = 94,
#else // 1.12.2
        FishingHook = -3,
        Player = -2,
        ItemEntity = 1,
        ExperienceOrb = 2,
        AreaEffectCloud = 3,
        ElderGuardian = 4,
        WitherSkeleton = 5,
        Stray = 6,
        ThrownEgg = 7,
        LeashFenceKnotEntity = 8,
        Painting = 9,
        Arrow = 10,
        Snowball = 11,
        LargeFireball = 12,
        SmallFireball = 13,
        ThrownEnderpearl = 14,
        EyeOfEnder = 15,
        ThrownPotion = 16,
        ThrownExperienceBottle = 17,
        ItemFrame = 18,
        WitherSkull = 19,
        PrimedTnt = 20,
        FallingBlockEntity = 21,
        FireworkRocketEntity = 22,
        Husk = 23,
        SpectralArrow = 24,
        ShulkerBullet = 25,
        DragonFireball = 26,
        ZombieVillager = 27,
        SkeletonHorse = 28,
        ZombieHorse = 29,
        ArmorStand = 30,
        Donkey = 31,
        Mule = 32,
        EvokerFangs = 33,
        Evoker = 34,
        Vex = 35,
        Vindicator = 36,
        Illusioner = 37,
        MinecartCommandBlock = 40,
        Boat = 41,
        Minecart = 42,
        MinecartChest = 43,
        MinecartFurnace = 44,
        MinecartTNT = 45,
        MinecartHopper = 46,
        MinecartSpawner = 47,
        Creeper = 50,
        Skeleton = 51,
        Spider = 52,
        Giant = 53,
        Zombie = 54,
        Slime = 55,
        Ghast = 56,
        PigZombie = 57,
        EnderMan = 58,
        CaveSpider = 59,
        Silverfish = 60,
        Blaze = 61,
        MagmaCube = 62,
        EnderDragon = 63,
        WitherBoss = 64,
        Bat = 65,
        Witch = 66,
        Endermite = 67,
        Guardian = 68,
        Shulker = 69,
        Pig = 90,
        Sheep = 91,
        Cow = 92,
        Chicken = 93,
        Squid = 94,
        Wolf = 95,
        MushroomCow = 96,
        SnowGolem = 97,
        Ocelot = 98,
        IronGolem = 99,
        Horse = 100,
        Rabbit = 101,
        PolarBear = 102,
        Llama = 103,
        LlamaSpit = 104,
        Parrot = 105,
        Villager = 120,
        EndCrystal = 200,
#endif
        MaxEntityIndex
    };

#if PROTOCOL_VERSION < 458
    enum class ObjectEntityType
    {
        None = -1,
        Boat = 1,
        ItemEntity = 2,
        AreaEffectCloud = 3,
        PrimedTnt = 50,
        EndCrystal = 51,
        Arrow = 60,
        Snowball = 61,
        ThrownEgg = 62,
        LargeFireball = 63,
        SmallFireball = 64,
        ThrownEnderpearl = 65,
        WitherSkull = 66,
        ShulkerBullet = 67,
        LlamaSpit = 68,
        FallingBlockEntity = 70,
        ItemFrame = 71,
        EyeOfEnder = 72,
        ThrownPotion = 73,
        ThrownExperienceBottle = 75,
        FireworkRocketEntity = 76,
        LeashFenceKnotEntity = 77,
        ArmorStand = 78,
        EvokerFangs = 79,
        FishingHook = 90,
        SpectralArrow = 91,
        DragonFireball = 93,
#if PROTOCOL_VERSION > 340
        ThrownTrident = 94,
#endif
        MaxEntityIndex
    };
#endif
}
