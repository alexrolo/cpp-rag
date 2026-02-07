#pragma once

#include "util/http_client/HttpClient.hpp"

namespace repositories
{
    namespace embedder
    {
        class EmbedderRepository
        {
        private:
            http::HttpClient http_client;

        public:
            explicit EmbedderRepository(http::HttpClient client) : http_client(
                                                                       std::move(client)) {}

            http::HttpResponse GetEmbedding(const std::string &input) const;
        };
    }
};
