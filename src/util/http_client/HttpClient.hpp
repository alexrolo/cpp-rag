#pragma once

#include <stdexcept>
#include <map>
#include <optional>
#include <string>

namespace http
{

    class HttpResponse
    {
    public:
        long status_code = 0;
        std::string body;
        std::map<std::string, std::string> headers;
        std::string request_url;
        std::string request_method;
        std::string request_body;

        void ThrowErrorIfFailed() const
        {
            if (status_code < 200 || status_code >= 300)
            {
                throw std::runtime_error("HTTP request failed with status code " + std::to_string(status_code) + ": " + body);
            }
        }
    };

    class HttpClient
    {
    public:
        explicit HttpClient(std::string base_url);

        HttpResponse Get(const std::string &path) const;
        HttpResponse Post(const std::string &path,
                          const std::optional<std::string> &json_body = std::nullopt) const;
        HttpResponse Put(const std::string &path,
                         const std::optional<std::string> &json_body = std::nullopt) const;
        HttpResponse Delete(const std::string &path) const;

    private:
        std::string base_url_;
    };

};