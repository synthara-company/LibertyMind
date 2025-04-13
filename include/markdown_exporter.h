#pragma once

#include <string>
#include <vector>
#include "api_client.h"

namespace libertymind {

class MarkdownExporter {
public:
    // Export chat history to a Markdown file
    static bool exportChatToMarkdown(const std::vector<Message>& messages, const std::string& filePath);
    
    // Generate Markdown content from chat history
    static std::string generateMarkdown(const std::vector<Message>& messages);
    
    // Generate a sample Markdown file
    static bool generateSampleMarkdown(const std::string& filePath);
};

} // namespace libertymind
