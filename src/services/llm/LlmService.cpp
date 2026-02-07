#include "./LlmService.hpp"
#include <cctype>
#include <sstream>

namespace services
{
    namespace llm
    {
        namespace
        {
            std::string RTrim(std::string value)
            {
                while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())) != 0)
                {
                    value.pop_back();
                }
                return value;
            }

            std::string TrimToLastSentence(const std::string &value)
            {
                std::string trimmed = RTrim(value);
                if (trimmed.empty())
                {
                    return trimmed;
                }

                char last_char = trimmed.back();
                if (last_char == '.' || last_char == '!' || last_char == '?')
                {
                    return trimmed;
                }

                const std::string sentence_endings = ".!?";
                std::string::size_type last_end = trimmed.find_last_of(sentence_endings);
                if (last_end == std::string::npos)
                {
                    return trimmed;
                }

                return trimmed.substr(0, last_end + 1);
            }
        }

        std::string LlmService::GenerateAnswer(const std::string &question,
                                               const std::vector<std::string> &context_documents,
                                               unsigned int max_tokens) const
        {
            // Build context from documents
            std::ostringstream context_builder;
            context_builder << "Context information:\\n";
            for (size_t i = 0; i < context_documents.size(); ++i)
            {
                context_builder << "[" << (i + 1) << "] " << context_documents[i] << "\\n";
            }
            context_builder << "\\n";

            unsigned int max_words = static_cast<unsigned int>(max_tokens * 0.75);
            if (max_words < 20)
            {
                max_words = 20;
            }

            // Build the prompt
            std::ostringstream prompt_builder;
            prompt_builder << "You are a helpful assistant. Answer the following question based on the provided context information. "
                           << "Keep the answer under about " << max_words << " words and finish your last sentence.\\n\\n"
                           << context_builder.str()
                           << "Question: " << question << "\\n"
                           << "Answer: ";

            std::string response = llm_repository.GenerateCompletion(prompt_builder.str(), max_tokens);
            return TrimToLastSentence(response);
        }
    }
};
