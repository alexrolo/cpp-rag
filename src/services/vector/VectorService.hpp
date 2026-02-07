#pragma once

#include "repositories/vector/VectorRepository.hpp"

namespace services
{
    namespace vector
    {
        class VectorService
        {
        private:
            repositories::vector::VectorRepository vector_repository;

        public:
            explicit VectorService(repositories::vector::VectorRepository repository) : vector_repository(std::move(repository)) {}

            util::http::HttpResponse CreateCollection(const std::string &collection_name, int vector_size) const;
            util::http::HttpResponse GetCollection(const std::string &collection_name) const;
            util::http::HttpResponse DeleteCollection(const std::string &collection_name) const;

            util::http::HttpResponse UpsertPoint(const std::string &collection_name, const repositories::vector::VectorPoint &point) const;
            util::http::HttpResponse UpsertPoints(const std::string &collection_name, const std::vector<repositories::vector::VectorPoint> &points) const;
            util::http::HttpResponse DeletePoint(const std::string &collection_name, int point_id) const;

            std::vector<repositories::vector::SearchResult> SearchSimilar(const std::string &collection_name,
                                                                          const std::vector<float> &query_vector,
                                                                          int limit = 10) const;
        };
    }
};