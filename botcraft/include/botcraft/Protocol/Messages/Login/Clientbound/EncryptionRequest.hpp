#pragma once

#include <iostream>

#include "botcraft/Protocol/BaseMessage.hpp"

namespace Botcraft
{
    class EncryptionRequest : public BaseMessage<EncryptionRequest>
    {
    public:
        virtual const int GetId() const override
        {
            return 0x01;
        }

        virtual const std::string GetName() const override
        {
            return "Encryption Request";
        }

        const std::string& GetServerID() const
        {
            return server_ID;
        }

        const std::vector<unsigned char>& GetPublicKey() const
        {
            return public_key;
        }

        const std::vector<unsigned char>& GetVerifyToken() const
        {
            return verify_token;
        }

    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override
        {
            server_ID = ReadString(iter, length);
            public_key_length = ReadVarInt(iter, length);
            public_key = ReadByteArray(iter, length, public_key_length);
            verify_token_length = ReadVarInt(iter, length);
            verify_token = ReadByteArray(iter, length, verify_token_length);
        }

        virtual void WriteImpl(WriteContainer &container) const override
        {
            std::cerr << "Clientbound message" << std::endl;
        }

        virtual const picojson::value SerializeImpl() const override
        {
            picojson::value value(picojson::object_type, false);
            picojson::object& object = value.get<picojson::object>();

            object["server_ID"] = picojson::value(server_ID);
            object["public_key_length"] = picojson::value((double)public_key_length);
            object["public_key"] = picojson::value("vector of " + std::to_string(public_key.size()) + " unsigned char");
            object["verify_token_length"] = picojson::value((double)verify_token_length);
            object["verify_token"] = picojson::value("vector of " + std::to_string(verify_token.size()) + " unsigned char");

            return value;
        }

    private:
        std::string server_ID;
        int public_key_length;
        std::vector<unsigned char> public_key;
        int verify_token_length;
        std::vector<unsigned char> verify_token;
    };
}