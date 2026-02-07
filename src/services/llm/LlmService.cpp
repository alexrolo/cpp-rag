#include "./LlmService.hpp"
#include <sstream>

namespace services
{
    http::HttpResponse LlmService::GenerateAnswer(const std::string &question,
                                                  const std::vector<std::string> &context_documents) const
    {
        // Build context from documents
        std::ostringstream context_builder;
        context_builder << "Context information:\\n";
        for (size_t i = 0; i < context_documents.size(); ++i)
        {
            context_builder << "[" << (i + 1) << "] " << context_documents[i] << "\\n";
        }
        context_builder << "\\n";

        // Build the prompt
        std::ostringstream prompt_builder;
        prompt_builder << "You are a helpful assistant. Answer the following question based on the provided context information.\\n\\n"
                       << context_builder.str()
                       << "Question: " << question << "\\n"
                       << "Answer: ";

        return llm_repository.GenerateCompletion(prompt_builder.str(), 128);
    }
};
