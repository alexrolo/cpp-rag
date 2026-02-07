#include <iostream>
#include <nlohmann/json.hpp>

#include "util/http_client/HttpClient.hpp"
#include "util/env/EnvLoader.hpp"

#include <string>
#include <vector>

using json = nlohmann::json;

#include "repositories/embedder/EmbedderRepository.hpp"
#include "services/embedder/EmbedderService.hpp"

#include "repositories/vector/VectorRepository.hpp"
#include "services/vector/VectorService.hpp"

#include "repositories/llm/LlmRepository.hpp"
#include "services/llm/LlmService.hpp"

void answer_query(
    const std::string &query,
    const services::embedder::EmbedderService &embedder_service,
    const services::vector::VectorService &vector_service,
    const services::llm::LlmService &llm_service,
    const std::string &collection_name)
{
    // Obtain embedding for query
    std::cout << "Query: " << query << std::endl;
    std::vector<float> embedding_vector = embedder_service.GetEmbedding(query);

    // Search for documents with similar embeddings to that of the query
    std::vector<repositories::vector::SearchResult> search_results = vector_service.SearchSimilar(collection_name, embedding_vector, 5);
    std::vector<std::string> context_documents;
    for (const auto &result : search_results)
    {
        std::cout << "[" << result.id << " - " << result.score << "] " << result.payload << std::endl;
        context_documents.push_back(result.payload);
    }

    // Generate answer using LLM based on retrieved context documents and the query
    if (!context_documents.empty())
    {
        std::cout << "Generating answer with " << context_documents.size() << " context documents..."
                  << std::endl;
        std::string response = llm_service.GenerateAnswer(query, context_documents);
        std::cout << "LLM Response Body: " << response << std::endl;
    }

    return;
}

int main()
{
    // Load environment variables from .env file
    util::env::EnvLoader env_loader;
    if (!env_loader.Load(".env"))
    {
        std::cerr << "Error: Could not load .env file" << std::endl;
        return 1;
    }

    // Declare services and repositories
    const std::string llm_url = env_loader.Get("LLM_SERVICE_URL", "http://localhost:8080");
    const std::string embedder_url = env_loader.Get("EMBEDDER_SERVICE_URL", "http://localhost:8081");
    const std::string vector_db_url = env_loader.Get("VECTOR_DB_URL", "http://localhost:6333");

    util::http::HttpClient llm_client(llm_url);
    util::http::HttpClient embedder_client(embedder_url);
    util::http::HttpClient vector_client(vector_db_url);

    repositories::llm::LlmRepository llm_repo(std::move(llm_client));
    repositories::embedder::EmbedderRepository embedder_repo(std::move(embedder_client));
    repositories::vector::VectorRepository vector_repo(std::move(vector_client));

    services::llm::LlmService llm_service(std::move(llm_repo));
    services::embedder::EmbedderService embedder_service(std::move(embedder_repo));
    services::vector::VectorService vector_service(std::move(vector_repo));

    const std::string collection_name = "test_collection";

    // Check if collection exists and delete it for a clean slate
    util::http::HttpResponse response = vector_service.GetCollection(collection_name);
    if (response.status_code == 200)
    {
        std::cout << "Collection 'test_collection' already exists. Deleting it first..." << std::endl;
        vector_service.DeleteCollection(collection_name);
    }

    // Example usage, set of documents to index and query
    const std::vector<std::string> documents = {
        "C++ is a powerful programming language.",
        "The cat is on the roof.",
        "Artificial intelligence is transforming the world.",
        "The quick brown fox jumps over the lazy dog.",
        "David Espejo is the most experienced fullstack developer who rarely leaves home.",
        "RAG stands for Retrieval-Augmented Generation.",
        "RAG uses a vector database to store and retrieve relevant information based on user queries.",
        "Ricardo Valle is an expert in low level programming and system design.",
        "RAG uses an LLM to generate responses based on retrieved information and user input.",
        "Alicia has stinky feet but is the only one who knows Power BI.",
        "Corporate knowledge article database are often bloated.",
        "Alejandro Rodriguez is a known Mariachi singer in Mexico.",
        "Corporate knowledge articles are often tagged incorrectly and hard to find.",
    };

    // Obtain embeddings for documents
    // std::string -> std::vector<float>
    std::vector<repositories::vector::VectorPoint> points;
    std::cout << "Collection contents:" << std::endl;
    for (int idx = 0; idx < documents.size(); ++idx)
    {
        const std::string &doc = documents[idx];
        std::cout << "[" << idx << "]\t" << doc << std::endl;
        std::vector<float> embedding_vector = embedder_service.GetEmbedding(doc);
        std::string j = json{{"text", doc}}.dump();
        points.push_back(repositories::vector::VectorPoint{idx, embedding_vector, j});
    }

    // Create collection
    response = vector_service.CreateCollection(collection_name, points[0].vector.size());
    vector_service.UpsertPoints(collection_name, points);

    // Queries
    std::cout << std::endl;
    std::cout << "########## QUERY 1 ##########" << std::endl;
    std::string query = "What is RAG and why is it useful in corporate?";
    answer_query(query, embedder_service, vector_service, llm_service, collection_name);

    std::cout << std::endl;
    std::cout << "########## QUERY 2 ##########" << std::endl;
    query = "Who are the members of my team and what are they known for?";
    answer_query(query, embedder_service, vector_service, llm_service, collection_name);

    return 0;
};
