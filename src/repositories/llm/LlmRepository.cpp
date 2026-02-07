#include "./LlmRepository.hpp"
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace repositories
{
    std::string LlmRepository::GenerateCompletion(const std::string &prompt, int max_tokens) const
    {
        const std::string path = "/v1/completions";

        std::ostringstream json_body;
        json_body << "{"
                  << "\"prompt\":\"" << prompt << "\""
                  << ",\"n_predict\":" << max_tokens
                  << ",\"temperature\":0.7"
                  << "}";

        http::HttpResponse response = http_client.Post(path, json_body.str());
        response.ThrowErrorIfFailed();

        json response_json = json::parse(response.body);
        if (!response_json.contains("choices") || !response_json["choices"].is_array() ||
            response_json["choices"].empty() || !response_json["choices"][0].contains("text") ||
            !response_json["choices"][0]["text"].is_string())
        {
            throw std::runtime_error("Invalid completion response format: " + response.body);
        }
        return response_json["choices"][0]["text"].get<std::string>();
    }
};
