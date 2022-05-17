#include <iostream>
#include <string>

#include <botcraft/AI/SimpleBehaviourClient.hpp>
#include <botcraft/Utilities/Logger.hpp>

#include "MobHitterTasks.hpp"

#include <chrono>
#include <thread>
#include <unistd.h>

void ShowHelp(const char* argv0)
{
    std::cout << "Usage: " << argv0 << " <options>\n"
        << "Options:\n"
        << "\t-h, --help\tShow this help message\n"
        << "\t--address\tAddress of the server you want to connect to, default: 127.0.0.1:25565\n"
        << "\t--login\t\tPlayer name in offline mode, login for Mojang account, empty for Microsoft account, default: BCChatCommand\n"
        << "\t--password\tMojang account password, empty for servers in offline mode or Microsoft account, default: empty\n"
        << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        // Init logging, log everything >= Info, only to console, no file
        Botcraft::Logger::GetInstance().SetLogLevel(Botcraft::LogLevel::Info);
        Botcraft::Logger::GetInstance().SetFilename("");
        // Add a name to this thread for logging
        Botcraft::Logger::GetInstance().RegisterThread("main");

        std::string address = "127.0.0.1";
        std::string login = "login_";
        std::string password = "";
        int ports_start = 0;
        int ports_count = 0;
        int bots = 1;

        if (argc == 1)
        {
            LOG_WARNING("No command arguments. Using default options.");
            ShowHelp(argv[0]);
        }

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help")
            {
                ShowHelp(argv[0]);
                return 0;
            }
            else if (arg == "--address")
            {
                if (i + 1 < argc)
                {
                    address = argv[++i];
                }
                else
                {
                    LOG_FATAL("--address requires an argument");
                    return 1;
                }
            }
            else if (arg == "--login")
            {
                if (i + 1 < argc)
                {
                    login = argv[++i];
                }
                else
                {
                    LOG_FATAL("--login requires an argument");
                    return 1;
                }
            }
            else if (arg == "--bots")
            {
                if (i + 1 < argc)
                {
                    bots = atoi(argv[++i]);
                }
                else
                {
                    LOG_FATAL("--bots requires an argument");
                    return 1;
                }
            }
            else if (arg == "--ports_start")
            {
                if (i + 1 < argc)
                {
                    ports_start = atoi(argv[++i]);
                }
                else
                {
                    LOG_FATAL("--ports_start requires an argument");
                    return 1;
                }
            }
            else if (arg == "--ports_count")
            {
                if (i + 1 < argc)
                {
                    ports_count = atoi(argv[++i]);
                }
                else
                {
                    LOG_FATAL("--ports_count requires an argument");
                    return 1;
                }
            }
            else if (arg == "--password")
            {
                if (i + 1 < argc)
                {
                    password = argv[++i];
                }
                else
                {
                    LOG_FATAL("--password requires an argument");
                    return 1;
                }
            }
        }

        auto mob_hitter_tree = Botcraft::Builder<Botcraft::SimpleBehaviourClient>()
            .sequence()
                .leaf(HitCloseHostiles)
                .leaf(CleanLastTimeHit)
                .leaf(TravelToMapCenter)
            .end()
            .build();

        Botcraft::SimpleBehaviourClient client(true);

        int player_no = 0;
        while (bots > 1 && fork() == 0) {
            player_no++;
            if (player_no >= bots)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        login = login + std::to_string(player_no);

        client.SetAutoRespawn(true);

        if (ports_start > 0 && ports_count > 0) {
            int port = ports_start + player_no % ports_count;
            address += ":" + std::to_string(port);

        }

        LOG_INFO("Starting bot " + login + " " + address);

        client.Connect(address, login, password);
        client.SetBehaviourTree(mob_hitter_tree);

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        client.RunBehaviourUntilClosed();

        client.Disconnect();

        return 0;
    }
    catch (std::exception& e)
    {
        LOG_FATAL("Exception: " << e.what());
        return 1;
    }
    catch (...)
    {
        LOG_FATAL("Unknown exception");
        return 2;
    }
}
