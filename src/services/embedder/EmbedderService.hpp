#pragma once

#include "repositories/embedder/EmbedderRepository.hpp"

namespace services
{
    class EmbedderService
    {
    private:
        repositories::EmbedderRepository embedder_repository;

    public:
        explicit EmbedderService(repositories::EmbedderRepository repository) : embedder_repository(std::move(repository)) {}

        http::HttpResponse GetEmbedding(const std::string &input) const;
    };
};
