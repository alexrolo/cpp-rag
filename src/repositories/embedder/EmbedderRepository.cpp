#include "EmbedderRepository.hpp"

namespace repositories
{
    namespace embedder
    {
        http::HttpResponse EmbedderRepository::GetEmbedding(const std::string &input) const
        {
            const std::string path = "/v1/embeddings";
            const std::string json_body = "{\"input\": \"" + input + "\"}";
            return http_client.Post(path, json_body);
        }
    }
};
