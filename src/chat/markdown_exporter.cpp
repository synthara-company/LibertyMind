#include "markdown_exporter.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace libertymind {

bool MarkdownExporter::exportChatToMarkdown(const std::vector<Message>& messages, const std::string& filePath) {
    try {
        // Generate Markdown content
        std::string markdownContent = generateMarkdown(messages);
        
        // Write to file
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
            return false;
        }
        
        file << markdownContent;
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error exporting chat to Markdown: " << e.what() << std::endl;
        return false;
    }
}

std::string MarkdownExporter::generateMarkdown(const std::vector<Message>& messages) {
    std::stringstream markdown;
    
    // Add title and timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream timeStr;
    timeStr << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    markdown << "# Synthara Chat Session\n\n";
    markdown << "Date: " << timeStr.str() << "\n\n";
    
    // Add system message if present
    bool hasSystemMessage = false;
    for (const auto& message : messages) {
        if (message.role == "system") {
            markdown << "## System Instructions\n\n";
            markdown << message.content << "\n\n";
            hasSystemMessage = true;
            break;
        }
    }
    
    // Add conversation
    markdown << "## Conversation\n\n";
    
    for (const auto& message : messages) {
        if (message.role == "system") {
            continue; // Already handled above
        }
        
        if (message.role == "user") {
            markdown << "### User\n\n";
        } else if (message.role == "assistant") {
            markdown << "### Assistant\n\n";
        } else {
            markdown << "### " << message.role << "\n\n";
        }
        
        markdown << message.content << "\n\n";
    }
    
    // Add footer
    markdown << "---\n";
    markdown << "Generated by Synthara - A terminal-based LLM interface\n";
    
    return markdown.str();
}

bool MarkdownExporter::generateSampleMarkdown(const std::string& filePath) {
    // Create a sample conversation
    std::vector<Message> sampleMessages = {
        {"system", "You are a helpful assistant."},
        {"user", "What is Markdown?"},
        {"assistant", "# Markdown\n\nMarkdown is a lightweight markup language with plain text formatting syntax. It's designed to be easy to write and easy to read, and can be converted to HTML and many other formats.\n\n## Features\n\n- **Headers**: Use # for headers\n- **Emphasis**: Use *italics* or **bold**\n- **Lists**: Create ordered and unordered lists\n- **Links**: Add [links](https://example.com)\n- **Images**: Include ![images](image.jpg)\n- **Code**: Format `inline code` or code blocks\n\n```python\nprint(\"Hello, Markdown!\")\n```\n\nMarkdown is widely used for documentation, README files, forum posts, and more."},
        {"user", "Can you show me how to create a table in Markdown?"},
        {"assistant", "# Markdown Tables\n\nIn Markdown, you can create tables using pipes and hyphens. Here's an example:\n\n```markdown\n| Header 1 | Header 2 | Header 3 |\n| -------- | -------- | -------- |\n| Cell 1   | Cell 2   | Cell 3   |\n| Cell 4   | Cell 5   | Cell 6   |\n```\n\nThis will render as:\n\n| Header 1 | Header 2 | Header 3 |\n| -------- | -------- | -------- |\n| Cell 1   | Cell 2   | Cell 3   |\n| Cell 4   | Cell 5   | Cell 6   |\n\nYou can align columns by adding colons to the separator line:\n\n```markdown\n| Left     | Center   | Right    |\n| :------- | :------: | -------: |\n| Aligned  | Aligned  | Aligned  |\n```\n\nThis creates left, center, and right alignment for each column."}
    };
    
    // Export to file
    return exportChatToMarkdown(sampleMessages, filePath);
}

} // namespace libertymind
