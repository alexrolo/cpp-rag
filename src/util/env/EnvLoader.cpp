#include "./EnvLoader.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace util
{
    namespace env
    {
        std::string Trim(const std::string &str)
        {
            size_t start = 0;
            while (start < str.size() && std::isspace(str[start]))
            {
                ++start;
            }
            size_t end = str.size();
            while (end > start && std::isspace(str[end - 1]))
            {
                --end;
            }
            return str.substr(start, end - start);
        }

        bool EnvLoader::Load(const std::string &filepath)
        {
            std::ifstream file(filepath);
            if (!file.is_open())
            {
                return false;
            }

            std::string line;
            while (std::getline(file, line))
            {
                // Skip empty lines and comments
                line = Trim(line);
                if (line.empty() || line[0] == '#')
                {
                    continue;
                }

                // Parse key=value
                const size_t delimiter_pos = line.find('=');
                if (delimiter_pos == std::string::npos)
                {
                    continue;
                }

                std::string key = Trim(line.substr(0, delimiter_pos));
                std::string value = Trim(line.substr(delimiter_pos + 1));

                // Remove surrounding quotes if present
                if ((value.front() == '"' && value.back() == '"') ||
                    (value.front() == '\'' && value.back() == '\''))
                {
                    value = value.substr(1, value.size() - 2);
                }

                env_vars[key] = value;
            }

            file.close();
            return true;
        }

        std::string EnvLoader::Get(const std::string &key, const std::string &default_value) const
        {
            const auto it = env_vars.find(key);
            if (it != env_vars.end())
            {
                return it->second;
            }
            return default_value;
        }

        bool EnvLoader::Contains(const std::string &key) const
        {
            return env_vars.find(key) != env_vars.end();
        }
    };
};
