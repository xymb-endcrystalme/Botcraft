#pragma once

#include <iostream>

#include "botcraft/Protocol/BaseMessage.hpp"

namespace Botcraft
{
    class LoginSuccess : public BaseMessage<LoginSuccess>
    {
    public:
        virtual const int GetId() const override
        {
            return 0x02;
        }

        virtual const std::string GetName() const override
        {
            return "Login Success";
        }

        const std::string& GetUUID() const
        {
            return uuid;
        }

        const std::string& GetUsername() const
        {
            return username;
        }

    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override
        {
            uuid = ReadString(iter, length);
            username = ReadString(iter, length);
        }

        virtual void WriteImpl(WriteContainer &container) const override
        {
            std::cerr << "Clientbound message" << std::endl;
        }

        virtual const picojson::value SerializeImpl() const override
        {
            picojson::value value(picojson::object_type, false);
            picojson::object& object = value.get<picojson::object>();

            object["uuid"] = picojson::value(uuid);
            object["username"] = picojson::value(username);

            return value;
        }

    private:
        std::string uuid;
        std::string username;
    };
} //Botcraft