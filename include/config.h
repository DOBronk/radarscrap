#pragma once
#include <string>
#include <algorithm>
#include <fstream>
#include <map>

namespace radarscraper
{
    class Config
    {
    public:
        Config(const std::string &filename)
        {
            readfile(filename);
        }
        // TODO: Make this ushort if the CPR library accepts it! Saves us 2 bytes of memory every func call :)
        // This should be an uint16_t / ushort (0-65535), most libaries work with signed int though
        inline const int db_port() { return std::stoi(_settings["db_port"]); }
        inline const std::string db_host() { return _settings["db_host"]; }
        inline const std::string db_user() { return _settings["db_user"]; }
        inline const std::string db_password() { return _settings["db_password"]; }
        inline const std::string db_schema() { return _settings["db_database"]; }
        inline const std::string target() { return _settings["target_url"]; }

    private:
        std::map<std::string, std::string> _settings;
        void readfile(const std::string &);
        void parseline(const std::string &);
    };

}