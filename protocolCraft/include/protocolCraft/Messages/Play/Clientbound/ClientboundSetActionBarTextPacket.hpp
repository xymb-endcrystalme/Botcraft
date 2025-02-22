#pragma once

#if PROTOCOL_VERSION > 754
#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/Chat.hpp"

namespace ProtocolCraft
{
    class ClientboundSetActionBarTextPacket : public BaseMessage<ClientboundSetActionBarTextPacket>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
            return 0x41;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
            return 0x41;
#else
#error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Set Action Bar Text";
        }

        virtual ~ClientboundSetActionBarTextPacket() override
        {

        }


        void SetText(const Chat& text_)
        {
            text = text_;
        }


        const Chat& GetText() const
        {
            return text;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            text.Read(iter, length);
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            text.Write(container);
        }

        virtual const nlohmann::json SerializeImpl() const override
        {
            nlohmann::json output;

            output["text"] = text.Serialize();

            return output;
        }

    private:
        Chat text;

    };
} //ProtocolCraft
#endif