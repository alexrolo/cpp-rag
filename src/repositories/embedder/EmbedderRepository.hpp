#pragma once

#include "util/http_client/HttpClient.hpp"

namespace repositories
{
    namespace embedder
    {
        class EmbedderRepository
        {
        private:
            util::http::HttpClient http_client;

        public:
            explicit EmbedderRepository(util::http::HttpClient client) : http_client(
                                                                             std::move(client)) {}

            util::http::HttpResponse GetEmbedding(const std::string &input) const;
        };
    }
};
