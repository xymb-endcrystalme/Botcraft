#pragma once

#include "botcraft/Game/Entities/entities/monster/ZombieEntity.hpp"
#if PROTOCOL_VERSION > 404
#include "botcraft/Game/Entities/VillagerData.hpp"
#endif

namespace Botcraft
{
    class ZombieVillagerEntity : public ZombieEntity
    {
    protected:
        static constexpr int metadata_count = 2;
        static const std::array<std::string, metadata_count> metadata_names;
        static constexpr int hierarchy_metadata_count = ZombieEntity::metadata_count + ZombieEntity::hierarchy_metadata_count;

    public:
        ZombieVillagerEntity();
        virtual ~ZombieVillagerEntity();

        // Object related stuff
        virtual std::string GetName() const override;
        virtual EntityType GetType() const override;
        virtual double GetWidth() const override;
        virtual double GetHeight() const override;

        // Static stuff, for easier comparison
        static std::string GetClassName();
        static EntityType GetClassType();


        virtual nlohmann::json Serialize() const override;

        // Metadata stuff
        virtual void SetMetadataValue(const int index, const std::any& value) override;

        bool GetDataConvertingId() const;
#if PROTOCOL_VERSION > 404
        const VillagerData& GetDataVillagerData() const;
#else
        int GetDataVillagerProfessionId() const;
#endif

        void SetDataConvertingId(const bool data_converting_id);
#if PROTOCOL_VERSION > 404
        void SetDataVillagerData(const VillagerData& data_villager_data);
#else
        void SetDataVillagerProfessionId(const int data_villager_profession_id);
#endif

    };
}
