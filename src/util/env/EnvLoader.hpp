#pragma once

#include <string>
#include <map>

namespace util
{
    class EnvLoader
    {
    private:
        std::map<std::string, std::string> env_vars;

    public:
        EnvLoader() = default;

        bool Load(const std::string &filepath);
        std::string Get(const std::string &key, const std::string &default_value = "") const;
        bool Contains(const std::string &key) const;
    };
};
