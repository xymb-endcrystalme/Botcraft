#pragma once

#include <iostream>

#include "botcraft/Protocol/BaseMessage.hpp"
#include "botcraft/Game/Enums.hpp"

#include "botcraft/Version.hpp"

namespace Botcraft
{
    class JoinGame : public BaseMessage<JoinGame>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 340 // 1.12.2
            return 0x23;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
            return 0x25;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
            return 0x25;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
			return 0x26;
#else
            #error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Join Game";
        }

        const int GetEntityId() const
        {
            return entity_id;
        }

        const unsigned char GetGamemode() const
        {
            return gamemode;
        }

        const Dimension GetDimension() const
        {
            return dimension;
        }

#if PROTOCOL_VERSION > 551
		const long long int GetHashedSeed() const
		{
			return hashed_seed;
		}
#endif

#if PROTOCOL_VERSION < 477
        const Difficulty GetDifficulty() const
        {
            return difficulty;
        }
#endif

        const unsigned char GetMaxPlayers() const
        {
            return max_players;
        }

        const std::string& GetLevelType() const
        {
            return level_type;
        }

#if PROTOCOL_VERSION >= 477
        const int GetViewDistance() const
        {
            return view_distance;
        }
#endif

        const bool GetReducedDebugInfo() const
        {
            return reduced_debug_info;
        }

#if PROTOCOL_VERSION > 565
		const bool GetEnableRespawnScreen() const
		{
			return enable_respawn_screen;
		}
#endif

    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override
        {
            entity_id = ReadData<int>(iter, length);
            gamemode = ReadData<unsigned char>(iter, length);
            dimension = (Dimension)ReadData<int>(iter, length);
#if PROTOCOL_VERSION > 551
			hashed_seed = ReadData<long long int>(iter, length);
#endif
#if PROTOCOL_VERSION < 477
            difficulty = (Difficulty)ReadData<unsigned char>(iter, length);
#endif
            max_players = ReadData<unsigned char>(iter, length);
            level_type = ReadString(iter, length);
#if PROTOCOL_VERSION >= 477
            view_distance = ReadVarInt(iter, length);
#endif
            reduced_debug_info = ReadData<bool>(iter, length);
#if PROTOCOL_VERSION > 565
			enable_respawn_screen = ReadData<bool>(iter, length);
#endif
        }

        virtual void WriteImpl(WriteContainer &container) const override
        {
            std::cerr << "Clientbound message" << std::endl;
        }

        virtual const picojson::value SerializeImpl() const override
        {
            picojson::value value(picojson::object_type, false);
            picojson::object& object = value.get<picojson::object>();

            object["entity_id"] = picojson::value((double)entity_id);
            object["gamemode"] = picojson::value((double)gamemode);
            object["dimension"] = picojson::value((double)dimension);
#if PROTOCOL_VERSION > 551
            object["hashed_seed"] = picojson::value((double)hashed_seed);
#endif
            object["difficulty"] = picojson::value((double)difficulty);
            object["max_players"] = picojson::value((double)max_players);
            object["level_type"] = picojson::value(level_type);
#if PROTOCOL_VERSION >= 477
            object["view_distance"] = picojson::value((double)view_distance);
#endif
            object["reduced_debug_info"] = picojson::value(reduced_debug_info);
#if PROTOCOL_VERSION > 565
            object["enable_respawn_screen"] = picojson::value(enable_respawn_screen);
#endif

            return value;
        }

    private:
        int entity_id;
        unsigned char gamemode;
        Dimension dimension;
#if PROTOCOL_VERSION > 551
		long long int hashed_seed;
#endif
        Difficulty difficulty;
        unsigned char max_players;
        std::string level_type;
#if PROTOCOL_VERSION >= 477
        int view_distance;
#endif
        bool reduced_debug_info;
#if PROTOCOL_VERSION > 565
		bool enable_respawn_screen;
#endif
    };
} //Botcraft