#pragma once

#include "protocolCraft/BaseMessage.hpp"

namespace ProtocolCraft
{
    class ClientboundUpdateMobEffectPacket : public BaseMessage<ClientboundUpdateMobEffectPacket>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 340 // 1.12.2
            return 0x4F;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
            return 0x52;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
            return 0x58;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
            return 0x5A;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16.0 or 1.16.1
            return 0x59;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
            return 0x59;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
            return 0x64;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
            return 0x65;
#else
#error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Update Mob Effect";
        }

        virtual ~ClientboundUpdateMobEffectPacket() override
        {

        }

        void SetEntityId(const int entity_id_)
        {
            entity_id = entity_id_;
        }

#if PROTOCOL_VERSION < 758
        void SetEffectId(const char effect_id_)
        {
            effect_id = effect_id_;
        }
#else
        void SetEffectId(const int effect_id_)
        {
            effect_id = effect_id_;
        }
#endif

        void SetEffectAmplifier(const char effect_amplifier_)
        {
            effect_amplifier = effect_amplifier_;
        }

        void SetEffectDurationTicks(const int effect_duration_ticks_)
        {
            effect_duration_ticks = effect_duration_ticks_;
        }

        void SetFlags(const char flags_)
        {
            flags = flags_;
        }


        const int GetEntityId() const
        {
            return entity_id;
        }

#if PROTOCOL_VERSION < 758
        const char GetEffectId() const
        {
            return effect_id;
        }
#else
        const int GetEffectId() const
        {
            return effect_id;
        }
#endif

        const char GetEffectAmplifier() const
        {
            return effect_amplifier;
        }

        const int GetEffectDurationTicks() const
        {
            return effect_duration_ticks;
        }

        const char GetFlags() const
        {
            return flags;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            entity_id = ReadData<VarInt>(iter, length);
#if PROTOCOL_VERSION < 758
            effect_id = ReadData<char>(iter, length);
#else
            effect_id = ReadData<VarInt>(iter, length);
#endif
            effect_amplifier = ReadData<char>(iter, length);
            effect_duration_ticks = ReadData<VarInt>(iter, length);
            flags = ReadData<char>(iter, length);
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<VarInt>(entity_id, container);
#if PROTOCOL_VERSION < 758
            WriteData<char>(effect_id, container);
#else
            WriteData<VarInt>(effect_id, container);
#endif
            WriteData<char>(effect_amplifier, container);
            WriteData<VarInt>(effect_duration_ticks, container);
            WriteData<char>(flags, container);
        }

        virtual const nlohmann::json SerializeImpl() const override
        {
            nlohmann::json output;

            output["entity_id"] = entity_id;
            output["effect_id"] = effect_id;
            output["effect_amplifier"] = effect_amplifier;
            output["effect_duration_ticks"] = effect_duration_ticks;
            output["flags"] = flags;

            return output;
        }

    private:
        int entity_id;
#if PROTOCOL_VERSION < 758
        char effect_id;
#else
        int effect_id;
#endif
        char effect_amplifier;
        int effect_duration_ticks;
        char flags;

    };
} //ProtocolCraft