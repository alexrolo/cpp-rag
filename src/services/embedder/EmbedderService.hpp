#pragma once

#include "repositories/embedder/EmbedderRepository.hpp"

#include <vector>

namespace services
{
    namespace embedder
    {
        class EmbedderService
        {
        private:
            repositories::embedder::EmbedderRepository embedder_repository;

        public:
            explicit EmbedderService(repositories::embedder::EmbedderRepository repository) : embedder_repository(std::move(repository)) {}

            std::vector<float> GetEmbedding(const std::string &input) const;
        };
    }
};
