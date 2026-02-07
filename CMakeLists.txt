cmake_minimum_required(VERSION 3.18)
project(rag LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(rag_app
  src/main.cpp
  src/util/http_client/HttpClient.cpp
  src/util/env/EnvLoader.cpp
  src/repositories/embedder/EmbedderRepository.cpp
  src/repositories/vector/VectorRepository.cpp
  src/repositories/llm/LlmRepository.cpp
  src/services/embedder/EmbedderService.cpp
  src/services/vector/VectorService.cpp
  src/services/llm/LlmService.cpp
)

target_include_directories(rag_app
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(rag_app
  PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)
