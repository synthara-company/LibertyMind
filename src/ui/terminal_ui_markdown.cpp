#include "terminal_ui.h"
#include "markdown_exporter.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
namespace libertymind {

void TerminalUI::drawMarkdownExport() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Export Chat to Markdown");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);
    
    // Draw input field
    mvwprintw(mainWindow, 2, 2, "Enter file path: ");
    mvwprintw(mainWindow, 2, 18, "%s", inputBuffer.c_str());
    
    // Draw instructions
    int y = 5;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Enter a file path to save the chat history as a Markdown file");
    mvwprintw(mainWindow, y++, 4, "Press Enter to save");
    mvwprintw(mainWindow, y++, 4, "Press Escape to cancel");
    
    // Draw sample path
    const char* homeDir = getenv("HOME");
    std::string samplePath = homeDir ? std::string(homeDir) + "/chat_export.md" : "./chat_export.md";
    mvwprintw(mainWindow, y++, 4, "Example: %s", samplePath.c_str());
    
    // Show cursor
    curs_set(1);
    wmove(mainWindow, 2, 18 + inputBuffer.length());
}

void TerminalUI::handleMarkdownExportInput(int key) {
    switch (key) {
        case '\n': // Enter key
            if (!inputBuffer.empty()) {
                // Get chat history from chat session
                const auto& history = chatSession->getHistory();
                
                // Export to Markdown
                bool success = MarkdownExporter::exportChatToMarkdown(history, inputBuffer);
                
                if (success) {
                    setStatusMessage("Chat history exported to " + inputBuffer);
                } else {
                    setStatusMessage("Error: Failed to export chat history");
                }
                
                // Return to chat screen
                currentScreen = Screen::CHAT;
                clearInputBuffer();
            }
            break;
        case 27: // Escape key
            currentScreen = Screen::CHAT;
            clearInputBuffer();
            break;
        case KEY_BACKSPACE:
        case 127: // Delete key
            backspaceInputBuffer();
            break;
        default:
            if (key >= 32 && key <= 126) { // Printable ASCII characters
                appendToInputBuffer(key);
            }
            break;
    }
}

} // namespace libertymind
