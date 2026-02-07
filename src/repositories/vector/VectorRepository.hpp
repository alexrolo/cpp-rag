#pragma once

#include "util/http_client/HttpClient.hpp"
#include <string>
#include <vector>

namespace repositories
{
    struct VectorPoint
    {
        int id;
        std::vector<float> vector;
        std::string payload; // JSON string
    };

    class VectorRepository
    {
    private:
        http::HttpClient http_client;

    public:
        explicit VectorRepository(http::HttpClient client) : http_client(std::move(client)) {}

        // Collection management
        http::HttpResponse CreateCollection(const std::string &collection_name, int vector_size) const;
        http::HttpResponse GetCollection(const std::string &collection_name) const;
        http::HttpResponse DeleteCollection(const std::string &collection_name) const;

        // Point operations
        http::HttpResponse UpsertPoint(const std::string &collection_name, const VectorPoint &point) const;
        http::HttpResponse UpsertPoints(const std::string &collection_name, const std::vector<VectorPoint> &points) const;
        http::HttpResponse DeletePoint(const std::string &collection_name, int point_id) const;

        // Search
        http::HttpResponse SearchSimilar(const std::string &collection_name,
                                         const std::vector<float> &query_vector,
                                         int limit = 10) const;
    };
};