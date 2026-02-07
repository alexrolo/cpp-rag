#pragma once

#include "repositories/llm/LlmRepository.hpp"
#include <string>
#include <vector>

namespace services
{
    class LlmService
    {
    private:
        repositories::LlmRepository llm_repository;

    public:
        explicit LlmService(repositories::LlmRepository repository) : llm_repository(std::move(repository)) {}

        http::HttpResponse GenerateAnswer(const std::string &question,
                                          const std::vector<std::string> &context_documents) const;
    };
};
