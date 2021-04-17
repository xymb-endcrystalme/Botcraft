#pragma once

#if PROTOCOL_VERSION > 471
#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/NetworkPosition.hpp"

namespace ProtocolCraft
{
    class ServerboundSetJigsawBlockPacket : public BaseMessage<ServerboundSetJigsawBlockPacket>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
            return 0x27;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
            return 0x27;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16.0 or 1.16.1
            return 0x28;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
            return 0x29;
#else
            #error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Set Jigsaw Block";
        }

        void SetPos(const NetworkPosition& pos_)
        {
            pos = pos_;
        }

#if PROTOCOL_VERSION > 708
        void SetName_(const Identifier& name__)
        {
            name_ = name__;
        }

        void SetTarget(const Identifier& target_)
        {
            target = target_;
        }

        void SetPool(const Identifier& pool_)
        {
            pool = pool_;
        }
#else
        void SetAttachmentType(const Identifier& attachment_type_)
        {
            attachment_type = attachment_type_;
        }

        void SetTargetPool(const Identifier& target_pool_)
        {
            target_pool = target_pool_;
        }
#endif

        void SetFinalState(const std::string& final_state_)
        {
            final_state = final_state_;
        }

#if PROTOCOL_VERSION > 708
        void SetJoint(const std::string& joint_)
        {
            joint = joint_;
        }
#endif

        const NetworkPosition& GetPos() const
        {
            return pos;
        }

#if PROTOCOL_VERSION > 708
        const Identifier& GetName_() const
        {
            return name_;
        }

        const Identifier& GetTarget() const
        {
            return target;
        }
        
        const Identifier& GetPool() const
        {
            return pool;
        }
#else
        const Identifier& GetAttachmentType() const
        {
            return attachment_type;
        }

        const Identifier& GetTargetPool() const
        {
            return target_pool;
        }
#endif

        const std::string& GetFinalState() const
        {
            return final_state;
        }

#if PROTOCOL_VERSION > 708
        const std::string& GetJoint() const
        {
            return joint;
        }
#endif


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            pos.Read(iter, length);
#if PROTOCOL_VERSION > 708
            name_ = ReadString(iter, length);
            target = ReadString(iter, length);
            pool = ReadString(iter, length);
#else
            attachment_type = ReadString(iter, length);
            target_pool = ReadString(iter, length);
#endif
            final_state = ReadString(iter, length);
#if PROTOCOL_VERSION > 708
            joint = ReadString(iter, length);
#endif
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            pos.Write(container);
#if PROTOCOL_VERSION > 708
            WriteString(name_, container);
            WriteString(target, container);
            WriteString(pool, container);
#else
            WriteString(attachment_type, container);
            WriteString(target_pool, container);
#endif
            WriteString(final_state, container);
#if PROTOCOL_VERSION > 708
            WriteString(joint, container);
#endif
        }

        virtual const picojson::value SerializeImpl() const override
        {
            picojson::value value(picojson::object_type, false);
            picojson::object& object = value.get<picojson::object>();

            object["pos"] = pos.Serialize();
#if PROTOCOL_VERSION > 708
            object["name_"] = picojson::value(name_);
            object["target"] = picojson::value(target);
            object["pool"] = picojson::value(pool);
#else
            object["attachment_type"] = picojson::value(attachment_type);
            object["target_pool"] = picojson::value(target_pool);
#endif
            object["final_state"] = picojson::value(final_state);
#if PROTOCOL_VERSION > 708
            object["joint"] = picojson::value(joint);
#endif

            return value;
        }

    private:
        NetworkPosition pos;
#if PROTOCOL_VERSION > 708
        Identifier name_;
        Identifier target;
        Identifier pool;
#else
        Identifier attachment_type;
        Identifier target_pool;
#endif
        std::string final_state;
#if PROTOCOL_VERSION > 708
        std::string joint;
#endif

    };
} //ProtocolCraft
#endif