#pragma once

#include "util/http_client/HttpClient.hpp"
#include <string>

namespace repositories
{
    class LlmRepository
    {
    private:
        http::HttpClient http_client;

    public:
        explicit LlmRepository(http::HttpClient client) : http_client(std::move(client)) {}

        std::string GenerateCompletion(const std::string &prompt, int max_tokens = 512) const;
    };
};
