#include "EmbedderService.hpp"

namespace services
{
    http::HttpResponse EmbedderService::GetEmbedding(const std::string &input) const
    {
        return embedder_repository.GetEmbedding(input);
    }
};
