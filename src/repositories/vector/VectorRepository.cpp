#include "VectorRepository.hpp"
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace repositories
{
    namespace vector
    {
        http::HttpResponse VectorRepository::CreateCollection(const std::string &collection_name, int vector_size) const
        {
            const std::string path = "/collections/" + collection_name;
            std::ostringstream json_body;
            json_body << "{\"vectors\":{\"size\":" << vector_size << ",\"distance\":\"Cosine\"}}";
            return http_client.Put(path, json_body.str());
        }

        http::HttpResponse VectorRepository::GetCollection(const std::string &collection_name) const
        {
            const std::string path = "/collections/" + collection_name;
            return http_client.Get(path);
        }

        http::HttpResponse VectorRepository::DeleteCollection(const std::string &collection_name) const
        {
            const std::string path = "/collections/" + collection_name;
            return http_client.Delete(path);
        }

        http::HttpResponse VectorRepository::UpsertPoint(const std::string &collection_name, const VectorPoint &point) const
        {
            const std::string path = "/collections/" + collection_name + "/points";

            std::ostringstream json_body;
            json_body << "{\"points\":[{\"id\":" << point.id << ",\"vector\":[";

            for (size_t i = 0; i < point.vector.size(); ++i)
            {
                if (i > 0)
                    json_body << ",";
                json_body << point.vector[i];
            }

            json_body << "],\"payload\":" << point.payload << "}]}";

            return http_client.Put(path, json_body.str());
        }

        http::HttpResponse VectorRepository::UpsertPoints(const std::string &collection_name, const std::vector<VectorPoint> &points) const
        {
            const std::string path = "/collections/" + collection_name + "/points";

            std::ostringstream json_body;
            json_body << "{\"points\":[";

            for (size_t p = 0; p < points.size(); ++p)
            {
                if (p > 0)
                    json_body << ",";

                json_body << "{\"id\":" << points[p].id << ",\"vector\":[";

                for (size_t i = 0; i < points[p].vector.size(); ++i)
                {
                    if (i > 0)
                        json_body << ",";
                    json_body << points[p].vector[i];
                }

                json_body << "],\"payload\":" << points[p].payload << "}";
            }

            json_body << "]}";

            return http_client.Put(path, json_body.str());
        }

        http::HttpResponse VectorRepository::DeletePoint(const std::string &collection_name, int point_id) const
        {
            const std::string path = "/collections/" + collection_name + "/points/delete";
            std::ostringstream json_body;
            json_body << "{\"points\":[" << point_id << "]}";
            return http_client.Post(path, json_body.str());
        }

        std::vector<SearchResult> VectorRepository::SearchSimilar(const std::string &collection_name,
                                                                  const std::vector<float> &query_vector,
                                                                  int limit) const
        {
            const std::string path = "/collections/" + collection_name + "/points/search";

            std::ostringstream json_body;
            json_body << "{\"vector\":[";

            for (size_t i = 0; i < query_vector.size(); ++i)
            {
                if (i > 0)
                    json_body << ",";
                json_body << query_vector[i];
            }

            json_body << "],\"limit\":" << limit << ",\"with_payload\":true}";

            http::HttpResponse response = http_client.Post(path, json_body.str());
            response.ThrowErrorIfFailed();

            std::vector<SearchResult> context_documents;
            json search_json = json::parse(response.body);
            for (const auto &result : search_json["result"])
            {
                const auto &payload = result["payload"];
                SearchResult point{
                    result["id"].get<int>(),
                    result["score"].get<float>(),
                    payload["text"].get<std::string>()};
                context_documents.push_back(point);
            }
            return context_documents;
        }
    }
};