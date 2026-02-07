#pragma once

#include "repositories/embedder/EmbedderRepository.hpp"

#include <vector>

namespace services
{
    class EmbedderService
    {
    private:
        repositories::EmbedderRepository embedder_repository;

    public:
        explicit EmbedderService(repositories::EmbedderRepository repository) : embedder_repository(std::move(repository)) {}

        std::vector<float> GetEmbedding(const std::string &input) const;
    };
};
