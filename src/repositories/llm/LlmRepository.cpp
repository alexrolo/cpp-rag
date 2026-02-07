#include "./LlmRepository.hpp"
#include <sstream>

namespace repositories
{
    http::HttpResponse LlmRepository::GenerateCompletion(const std::string &prompt, int max_tokens) const
    {
        const std::string path = "/v1/completions";
        
        std::ostringstream json_body;
        json_body << "{"
                  << "\"prompt\":\"" << prompt << "\""
                  << ",\"n_predict\":" << max_tokens
                  << ",\"temperature\":0.7"
                  << "}";
        
        return http_client.Post(path, json_body.str());
    }
};
