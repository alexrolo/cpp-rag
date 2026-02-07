#include "EmbedderService.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace services
{
    namespace embedder
    {
        std::vector<float> EmbedderService::GetEmbedding(const std::string &input) const
        {
            http::HttpResponse response = embedder_repository.GetEmbedding(input);
            response.ThrowErrorIfFailed();

            json response_json = json::parse(response.body);
            if (
                !response_json.contains("data") || !response_json["data"].is_array() ||
                response_json["data"].empty() || !response_json["data"][0].contains("embedding") ||
                !response_json["data"][0]["embedding"].is_array())
            {
                throw std::runtime_error("Invalid embedding response format: " + response.body);
            }
            const auto &embedding = response_json["data"][0]["embedding"];
            return embedding.get<std::vector<float>>();
        };
    }
};
