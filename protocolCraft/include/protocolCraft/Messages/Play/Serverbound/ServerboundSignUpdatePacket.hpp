#pragma once

#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/NetworkPosition.hpp"

namespace ProtocolCraft
{
    class ServerboundSignUpdatePacket : public BaseMessage<ServerboundSignUpdatePacket>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 340 // 1.12.2
            return 0x1C;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
            return 0x26;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
            return 0x29;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
            return 0x29;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16.0 or 1.16.1
            return 0x2A;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
            return 0x2B;
#elif PROTOCOL_VERSION == 755 // 1.17
            return 0x2B;
#else
            #error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Sign Update";
        }

        virtual ~ServerboundSignUpdatePacket() override
        {

        }

        void SetPos(const NetworkPosition& pos_)
        {
            pos = pos_;
        }

        void SetLines(const std::vector<std::string>& lines_)
        {
            lines = lines_;
        }


        const NetworkPosition& GetPos() const
        {
            return pos;
        }

        const std::vector<std::string>& GetLines() const
        {
            return lines;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            pos.Read(iter, length);
            lines = std::vector<std::string>(4);
            for (int i = 0; i < 4; ++i)
            {
                lines[i] = ReadString(iter, length);
            }
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            pos.Write(container);
            for (int i = 0; i < 4; ++i)
            {
                WriteString(lines[i], container);
            }
        }

        virtual const picojson::value SerializeImpl() const override
        {
            picojson::value value(picojson::object_type, false);
            picojson::object& object = value.get<picojson::object>();

            object["pos"] = pos.Serialize();

            object["lines"] = picojson::value(picojson::array_type, false);
            picojson::array& array = object["lines"].get<picojson::array>();
            for (int i = 0; i < 4; ++i)
            {
                array.push_back(picojson::value(lines[i]));
            }

            return value;
        }

    private:
        NetworkPosition pos;
        std::vector<std::string> lines;

    };
} //ProtocolCraft