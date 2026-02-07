#pragma once

#include "util/http_client/HttpClient.hpp"
#include <string>
#include <vector>

namespace repositories
{
    namespace vector
    {
        struct VectorPoint
        {
            int id;
            std::vector<float> vector;
            std::string payload; // JSON string
        };

        class SearchResult
        {
        public:
            int id;
            float score;
            std::string payload;
        };

        class VectorRepository
        {
        private:
            util::http::HttpClient http_client;

        public:
            explicit VectorRepository(util::http::HttpClient client) : http_client(std::move(client)) {}

            // Collection management
            util::http::HttpResponse CreateCollection(const std::string &collection_name, int vector_size) const;
            util::http::HttpResponse GetCollection(const std::string &collection_name) const;
            util::http::HttpResponse DeleteCollection(const std::string &collection_name) const;

            // Point operations
            util::http::HttpResponse UpsertPoint(const std::string &collection_name, const VectorPoint &point) const;
            util::http::HttpResponse UpsertPoints(const std::string &collection_name, const std::vector<VectorPoint> &points) const;
            util::http::HttpResponse DeletePoint(const std::string &collection_name, int point_id) const;

            // Search
            std::vector<SearchResult> SearchSimilar(const std::string &collection_name,
                                                    const std::vector<float> &query_vector,
                                                    int limit = 10) const;
        };
    }
};