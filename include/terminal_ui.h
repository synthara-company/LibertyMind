#pragma once

#include "config_manager.h"
#include "model_registry.h"
#include "chat_session.h"
#include <memory>
#include <string>
#include <vector>
#include <ncurses.h>

namespace libertymind {

enum class Screen {
    MAIN_MENU,
    PROVIDER_SELECTION,
    MODEL_SELECTION,
    API_KEY_INPUT,
    CHAT,
    SYSTEM_MESSAGE,
    THEME_CUSTOMIZATION,
    MARKDOWN_EXPORT,
    COMPANY_INFO
};

class TerminalUI {
public:
    TerminalUI();
    ~TerminalUI();

    // Run the UI
    void run();

private:
    // Theme structure
    struct Theme {
        int headerFg;
        int headerBg;
        int selectedFg;
        int selectedBg;
        int userMsgFg;
        int assistantMsgFg;
        int errorMsgFg;

        // Default theme
        Theme() :
            headerFg(COLOR_WHITE), headerBg(COLOR_BLUE),
            selectedFg(COLOR_BLACK), selectedBg(COLOR_WHITE),
            userMsgFg(COLOR_GREEN), assistantMsgFg(COLOR_CYAN),
            errorMsgFg(COLOR_RED) {}
    };

    // UI state
    Screen currentScreen;
    int selectedOption;
    int scrollOffset;
    std::string inputBuffer;
    std::string statusMessage;
    bool running;
    Theme currentTheme;

    // Components
    std::shared_ptr<ConfigManager> configManager;
    std::unique_ptr<ModelRegistry> modelRegistry;
    std::unique_ptr<ChatSession> chatSession;

    // Windows
    WINDOW* mainWindow;
    WINDOW* inputWindow;
    WINDOW* statusWindow;

    // Initialize ncurses
    void initNcurses();

    // Clean up ncurses
    void cleanupNcurses();

    // Draw the current screen
    void drawScreen();

    // Handle input
    void handleInput();

    // Screen-specific drawing functions
    void drawMainMenu();
    void drawProviderSelection();
    void drawModelSelection();
    void drawApiKeyInput();
    void drawChat();
    void drawSystemMessage();
    void drawThemeCustomization();
    void drawMarkdownExport();
    void drawCompanyInfo();

    // Screen-specific input handlers
    void handleMainMenuInput(int key);
    void handleProviderSelectionInput(int key);
    void handleModelSelectionInput(int key);
    void handleApiKeyInputInput(int key);
    void handleChatInput(int key);
    void handleSystemMessageInput(int key);
    void handleThemeCustomizationInput(int key);
    void handleMarkdownExportInput(int key);
    void handleCompanyInfoInput(int key);

    // Helper functions
    void setStatusMessage(const std::string& message);
    void clearStatusMessage();
    std::string getProviderName(Provider provider);
    void refreshChatDisplay();
    void sendChatMessage(const std::string& message);
    void clearInputBuffer();
    void appendToInputBuffer(int key);
    void backspaceInputBuffer();

    // Theme management
    void initializeColorPairs();
    bool saveTheme() const;
    bool loadTheme();
    void applyTheme();
};

} // namespace libertymind
