#if PROTOCOL_VERSION > 578
#pragma once

#include "botcraft/Game/Entities/entities/monster/ZombieEntity.hpp"

namespace Botcraft
{
    class ZombifiedPiglinEntity : public ZombieEntity
    {
    protected:
        static constexpr int metadata_count = 0;
        static constexpr int hierarchy_metadata_count = ZombieEntity::metadata_count + ZombieEntity::hierarchy_metadata_count;

    public:
        ZombifiedPiglinEntity();
        virtual ~ZombifiedPiglinEntity();

        // Object related stuff
        virtual std::string GetName() const override;
        virtual EntityType GetType() const override;
        virtual double GetWidth() const override;
        virtual double GetHeight() const override;

        // Static stuff, for easier comparison
        static std::string GetClassName();
        static EntityType GetClassType();

    };
}
#endif
