#pragma once

#include "repositories/vector/VectorRepository.hpp"

namespace services
{
    class VectorService
    {
    private:
        repositories::VectorRepository vector_repository;

    public:
        explicit VectorService(repositories::VectorRepository repository) : vector_repository(std::move(repository)) {}

        http::HttpResponse CreateCollection(const std::string &collection_name, int vector_size) const;
        http::HttpResponse GetCollection(const std::string &collection_name) const;
        http::HttpResponse DeleteCollection(const std::string &collection_name) const;

        http::HttpResponse UpsertPoint(const std::string &collection_name, const repositories::VectorPoint &point) const;
        http::HttpResponse UpsertPoints(const std::string &collection_name, const std::vector<repositories::VectorPoint> &points) const;
        http::HttpResponse DeletePoint(const std::string &collection_name, int point_id) const;

        http::HttpResponse SearchSimilar(const std::string &collection_name,
                                         const std::vector<float> &query_vector,
                                         int limit = 10) const;
    };
};