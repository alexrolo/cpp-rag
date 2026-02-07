#include "VectorService.hpp"

namespace services
{
    namespace vector
    {
        util::http::HttpResponse VectorService::CreateCollection(const std::string &collection_name, int vector_size) const
        {
            return vector_repository.CreateCollection(collection_name, vector_size);
        }

        util::http::HttpResponse VectorService::GetCollection(const std::string &collection_name) const
        {
            return vector_repository.GetCollection(collection_name);
        }

        util::http::HttpResponse VectorService::DeleteCollection(const std::string &collection_name) const
        {
            return vector_repository.DeleteCollection(collection_name);
        }

        util::http::HttpResponse VectorService::UpsertPoint(const std::string &collection_name, const repositories::vector::VectorPoint &point) const
        {
            return vector_repository.UpsertPoint(collection_name, point);
        }

        util::http::HttpResponse VectorService::UpsertPoints(const std::string &collection_name, const std::vector<repositories::vector::VectorPoint> &points) const
        {
            return vector_repository.UpsertPoints(collection_name, points);
        }

        util::http::HttpResponse VectorService::DeletePoint(const std::string &collection_name, int point_id) const
        {
            return vector_repository.DeletePoint(collection_name, point_id);
        }

        std::vector<repositories::vector::SearchResult> VectorService::SearchSimilar(const std::string &collection_name,
                                                                                     const std::vector<float> &query_vector,
                                                                                     int limit) const
        {
            return vector_repository.SearchSimilar(collection_name, query_vector, limit);
        }
    }
};
