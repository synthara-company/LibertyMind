#include "terminal_ui.h"
#include <algorithm>
#include <iostream>
#include <sstream>

namespace libertymind {

TerminalUI::TerminalUI()
    : currentScreen(Screen::MAIN_MENU),
      selectedOption(0),
      scrollOffset(0),
      running(true) {

    // Initialize components
    configManager = std::make_shared<ConfigManager>();
    modelRegistry = std::make_unique<ModelRegistry>();
    chatSession = std::make_unique<ChatSession>(configManager);

    // Initialize ncurses
    initNcurses();
}

TerminalUI::~TerminalUI() {
    cleanupNcurses();
}

void TerminalUI::initNcurses() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    // Load theme if available, otherwise use default
    loadTheme();

    // Initialize color pairs based on theme
    initializeColorPairs();

    // Create windows
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    mainWindow = newwin(maxY - 3, maxX, 0, 0);
    inputWindow = newwin(2, maxX, maxY - 3, 0);
    statusWindow = newwin(1, maxX, maxY - 1, 0);

    // Enable scrolling for main window
    scrollok(mainWindow, TRUE);

    // Draw box around input window
    box(inputWindow, 0, 0);

    // Refresh windows
    refresh();
    wrefresh(mainWindow);
    wrefresh(inputWindow);
    wrefresh(statusWindow);
}

void TerminalUI::cleanupNcurses() {
    // Delete windows
    delwin(mainWindow);
    delwin(inputWindow);
    delwin(statusWindow);

    // End ncurses
    endwin();
}

void TerminalUI::run() {
    while (running) {
        // Draw the current screen
        drawScreen();

        // Handle input
        handleInput();
    }
}

void TerminalUI::drawScreen() {
    // Clear windows
    wclear(mainWindow);
    wclear(inputWindow);
    wclear(statusWindow);

    // Draw box around input window
    box(inputWindow, 0, 0);

    // Draw status message
    wattron(statusWindow, A_BOLD);
    mvwprintw(statusWindow, 0, 0, "%s", statusMessage.c_str());
    wattroff(statusWindow, A_BOLD);

    // Draw the current screen
    switch (currentScreen) {
        case Screen::MAIN_MENU:
            drawMainMenu();
            break;
        case Screen::PROVIDER_SELECTION:
            drawProviderSelection();
            break;
        case Screen::MODEL_SELECTION:
            drawModelSelection();
            break;
        case Screen::API_KEY_INPUT:
            drawApiKeyInput();
            break;
        case Screen::CHAT:
            drawChat();
            break;
        case Screen::SYSTEM_MESSAGE:
            drawSystemMessage();
            break;
        case Screen::THEME_CUSTOMIZATION:
            drawThemeCustomization();
            break;
        case Screen::MARKDOWN_EXPORT:
            drawMarkdownExport();
            break;
        case Screen::COMPANY_INFO:
            drawCompanyInfo();
            break;
    }

    // Refresh windows
    wrefresh(mainWindow);
    wrefresh(inputWindow);
    wrefresh(statusWindow);
}

void TerminalUI::handleInput() {
    int key = getch();

    // Handle global keys
    if (key == KEY_F(10)) {
        running = false;
        return;
    }

    // Handle screen-specific input
    switch (currentScreen) {
        case Screen::MAIN_MENU:
            handleMainMenuInput(key);
            break;
        case Screen::PROVIDER_SELECTION:
            handleProviderSelectionInput(key);
            break;
        case Screen::MODEL_SELECTION:
            handleModelSelectionInput(key);
            break;
        case Screen::API_KEY_INPUT:
            handleApiKeyInputInput(key);
            break;
        case Screen::CHAT:
            handleChatInput(key);
            break;
        case Screen::SYSTEM_MESSAGE:
            handleSystemMessageInput(key);
            break;
        case Screen::THEME_CUSTOMIZATION:
            handleThemeCustomizationInput(key);
            break;
        case Screen::MARKDOWN_EXPORT:
            handleMarkdownExportInput(key);
            break;
        case Screen::COMPANY_INFO:
            handleCompanyInfoInput(key);
            break;
    }
}

void TerminalUI::drawMainMenu() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Synthara - Terminal LLM Interface");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Draw menu options
    const std::vector<std::string> options = {
        "Select Provider",
        "Select Model",
        "Set API Key",
        "Start Chat",
        "Set System Message",
        "Customize Theme",
        "Company Info",
        "Exit"
    };

    for (size_t i = 0; i < options.size(); ++i) {
        if (static_cast<int>(i) == selectedOption) {
            wattron(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }

        mvwprintw(mainWindow, i + 2, 2, "%s", options[i].c_str());

        if (static_cast<int>(i) == selectedOption) {
            wattroff(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }
    }

    // Draw current configuration
    int y = options.size() + 4;
    mvwprintw(mainWindow, y++, 2, "Current Configuration:");
    mvwprintw(mainWindow, y++, 4, "Provider: %s", getProviderName(configManager->getSelectedProvider()).c_str());
    mvwprintw(mainWindow, y++, 4, "Model: %s", configManager->getSelectedModel().c_str());

    auto apiKey = configManager->getApiKey(configManager->getSelectedProvider());
    mvwprintw(mainWindow, y++, 4, "API Key: %s", apiKey ? "Set" : "Not Set");

    // Draw instructions
    y += 2;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Use arrow keys to navigate");
    mvwprintw(mainWindow, y++, 4, "Press Enter to select");
    mvwprintw(mainWindow, y++, 4, "Press F10 to exit");
}

void TerminalUI::drawProviderSelection() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Select Provider");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Get available providers
    std::vector<Provider> providers = modelRegistry->getAvailableProviders();

    // Draw provider options
    for (size_t i = 0; i < providers.size(); ++i) {
        if (static_cast<int>(i) == selectedOption) {
            wattron(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }

        mvwprintw(mainWindow, i + 2, 2, "%s", getProviderName(providers[i]).c_str());

        if (static_cast<int>(i) == selectedOption) {
            wattroff(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }
    }

    // Draw instructions
    int y = providers.size() + 4;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Use arrow keys to navigate");
    mvwprintw(mainWindow, y++, 4, "Press Enter to select");
    mvwprintw(mainWindow, y++, 4, "Press Escape to go back");
}

void TerminalUI::drawModelSelection() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Select Model");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Get available models for the selected provider
    Provider provider = configManager->getSelectedProvider();
    std::vector<ModelInfo> models = modelRegistry->getModelsForProvider(provider);

    // Draw model options
    for (size_t i = 0; i < models.size(); ++i) {
        if (static_cast<int>(i) == selectedOption) {
            wattron(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }

        mvwprintw(mainWindow, i + 2, 2, "%s - %s", models[i].id.c_str(), models[i].name.c_str());

        if (static_cast<int>(i) == selectedOption) {
            wattroff(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }
    }

    // Draw instructions
    int y = models.size() + 4;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Use arrow keys to navigate");
    mvwprintw(mainWindow, y++, 4, "Press Enter to select");
    mvwprintw(mainWindow, y++, 4, "Press Escape to go back");
}

void TerminalUI::drawApiKeyInput() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Set API Key for %s", getProviderName(configManager->getSelectedProvider()).c_str());
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Draw input field
    mvwprintw(mainWindow, 2, 2, "Enter API Key: ");

    // Display masked API key
    std::string maskedKey;
    for (size_t i = 0; i < inputBuffer.length(); ++i) {
        maskedKey += '*';
    }
    mvwprintw(mainWindow, 2, 17, "%s", maskedKey.c_str());

    // Draw instructions
    int y = 5;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Enter your API key for %s", getProviderName(configManager->getSelectedProvider()).c_str());
    mvwprintw(mainWindow, y++, 4, "Press Enter to save");
    mvwprintw(mainWindow, y++, 4, "Press Escape to cancel");

    // Add provider-specific help
    if (configManager->getSelectedProvider() == Provider::GOOGLE) {
        y++;
        wattron(mainWindow, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(mainWindow, y++, 2, "Google API Key Help:");
        wattroff(mainWindow, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(mainWindow, y++, 4, "Get your API key from Google AI Studio:");
        mvwprintw(mainWindow, y++, 4, "https://aistudio.google.com/");
        mvwprintw(mainWindow, y++, 4, "1. Create a free account");
        mvwprintw(mainWindow, y++, 4, "2. Go to API Keys section");
        mvwprintw(mainWindow, y++, 4, "3. Create a new API key");
    }

    // Show cursor
    curs_set(1);
    wmove(mainWindow, 2, 17 + maskedKey.length());
}

void TerminalUI::drawChat() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Synthara Chat - %s - %s",
              getProviderName(configManager->getSelectedProvider()).c_str(),
              configManager->getSelectedModel().c_str());
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Get chat history
    const std::vector<Message>& history = chatSession->getHistory();

    // Draw chat history
    int y = 1;
    for (size_t i = 1; i < history.size(); ++i) { // Skip system message
        const auto& message = history[i];

        if (message.role == "user") {
            wattron(mainWindow, COLOR_PAIR(3) | A_BOLD);
            mvwprintw(mainWindow, y++, 1, "You:");
            wattroff(mainWindow, COLOR_PAIR(3) | A_BOLD);
        } else if (message.role == "assistant") {
            wattron(mainWindow, COLOR_PAIR(4) | A_BOLD);
            mvwprintw(mainWindow, y++, 1, "Assistant:");
            wattroff(mainWindow, COLOR_PAIR(4) | A_BOLD);
        }

        // Word wrap the message content
        std::istringstream iss(message.content);
        std::string word;
        std::string line;
        int maxWidth = getmaxx(mainWindow) - 4;

        while (iss >> word) {
            if (line.length() + word.length() + 1 > static_cast<size_t>(maxWidth)) {
                mvwprintw(mainWindow, y++, 2, "%s", line.c_str());
                line = word;
            } else {
                if (!line.empty()) {
                    line += " ";
                }
                line += word;
            }
        }

        if (!line.empty()) {
            mvwprintw(mainWindow, y++, 2, "%s", line.c_str());
        }

        y++; // Add a blank line between messages
    }

    // Draw input prompt
    mvwprintw(inputWindow, 0, 2, "Enter message (Esc: menu, M: export to Markdown):");
    mvwprintw(inputWindow, 1, 2, "%s", inputBuffer.c_str());

    // Show cursor
    curs_set(1);
    wmove(inputWindow, 1, 2 + inputBuffer.length());
}

void TerminalUI::drawSystemMessage() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Set System Message");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Draw current system message
    mvwprintw(mainWindow, 2, 2, "Current system message:");
    mvwprintw(mainWindow, 3, 4, "%s", chatSession->getSystemMessage().c_str());

    // Draw input field
    mvwprintw(mainWindow, 5, 2, "New system message:");
    mvwprintw(mainWindow, 6, 4, "%s", inputBuffer.c_str());

    // Draw instructions
    int y = 9;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Enter a new system message to set the behavior of the assistant");
    mvwprintw(mainWindow, y++, 4, "Press Enter to save");
    mvwprintw(mainWindow, y++, 4, "Press Escape to cancel");

    // Show cursor
    curs_set(1);
    wmove(mainWindow, 6, 4 + inputBuffer.length());
}

void TerminalUI::handleMainMenuInput(int key) {
    switch (key) {
        case KEY_UP:
            selectedOption = std::max(0, selectedOption - 1);
            break;
        case KEY_DOWN:
            selectedOption = std::min(7, selectedOption + 1);
            break;
        case '\n': // Enter key
            switch (selectedOption) {
                case 0: // Select Provider
                    currentScreen = Screen::PROVIDER_SELECTION;
                    selectedOption = 0;
                    break;
                case 1: // Select Model
                    currentScreen = Screen::MODEL_SELECTION;
                    selectedOption = 0;
                    break;
                case 2: // Set API Key
                    currentScreen = Screen::API_KEY_INPUT;
                    clearInputBuffer();
                    break;
                case 3: // Start Chat
                {
                    Provider provider = configManager->getSelectedProvider();
                    auto apiKey = configManager->getApiKey(provider);

                    if (!apiKey) {
                        setStatusMessage("Error: API key not set for " + getProviderName(provider));
                    } else {
                        currentScreen = Screen::CHAT;
                        clearInputBuffer();
                        clearStatusMessage();
                    }
                    break;
                }
                case 4: // Set System Message
                    currentScreen = Screen::SYSTEM_MESSAGE;
                    inputBuffer = chatSession->getSystemMessage();
                    break;
                case 5: // Customize Theme
                    currentScreen = Screen::THEME_CUSTOMIZATION;
                    selectedOption = 0;
                    break;
                case 6: // Company Info
                    currentScreen = Screen::COMPANY_INFO;
                    break;
                case 7: // Exit
                    running = false;
                    break;
            }
            break;
    }
}

void TerminalUI::handleProviderSelectionInput(int key) {
    std::vector<Provider> providers = modelRegistry->getAvailableProviders();

    switch (key) {
        case KEY_UP:
            selectedOption = std::max(0, selectedOption - 1);
            break;
        case KEY_DOWN:
            selectedOption = std::min(static_cast<int>(providers.size()) - 1, selectedOption + 1);
            break;
        case '\n': // Enter key
            if (selectedOption >= 0 && selectedOption < static_cast<int>(providers.size())) {
                configManager->setSelectedProvider(providers[selectedOption]);
                currentScreen = Screen::MAIN_MENU;
                selectedOption = 0;
            }
            break;
        case 27: // Escape key
            currentScreen = Screen::MAIN_MENU;
            selectedOption = 0;
            break;
    }
}

void TerminalUI::handleModelSelectionInput(int key) {
    Provider provider = configManager->getSelectedProvider();
    std::vector<ModelInfo> models = modelRegistry->getModelsForProvider(provider);

    switch (key) {
        case KEY_UP:
            selectedOption = std::max(0, selectedOption - 1);
            break;
        case KEY_DOWN:
            selectedOption = std::min(static_cast<int>(models.size()) - 1, selectedOption + 1);
            break;
        case '\n': // Enter key
            if (selectedOption >= 0 && selectedOption < static_cast<int>(models.size())) {
                configManager->setSelectedModel(models[selectedOption].id);
                currentScreen = Screen::MAIN_MENU;
                selectedOption = 0;
            }
            break;
        case 27: // Escape key
            currentScreen = Screen::MAIN_MENU;
            selectedOption = 0;
            break;
    }
}

void TerminalUI::handleApiKeyInputInput(int key) {
    switch (key) {
        case '\n': // Enter key
            if (!inputBuffer.empty()) {
                Provider provider = configManager->getSelectedProvider();
                configManager->setApiKey(provider, inputBuffer);
                setStatusMessage("API key set for " + getProviderName(provider));
                currentScreen = Screen::MAIN_MENU;
                selectedOption = 0;
            }
            break;
        case 27: // Escape key
            currentScreen = Screen::MAIN_MENU;
            selectedOption = 0;
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

void TerminalUI::handleChatInput(int key) {
    switch (key) {
        case '\n': // Enter key
            if (!inputBuffer.empty()) {
                sendChatMessage(inputBuffer);
                clearInputBuffer();
            }
            break;
        case 27: // Escape key
            currentScreen = Screen::MAIN_MENU;
            selectedOption = 0;
            break;
        case 'm': // 'm' key for Markdown export (when Ctrl is pressed)
        case 'M': // 'M' key for Markdown export
            if (key == 'm' && !(key & 0x1F)) { // Check if Ctrl is pressed
                currentScreen = Screen::MARKDOWN_EXPORT;
                clearInputBuffer();
                // Set default filename
                const char* homeDir = getenv("HOME");
                inputBuffer = homeDir ? std::string(homeDir) + "/chat_export.md" : "./chat_export.md";
            } else if (key == 'M') {
                currentScreen = Screen::MARKDOWN_EXPORT;
                clearInputBuffer();
                // Set default filename
                const char* homeDir = getenv("HOME");
                inputBuffer = homeDir ? std::string(homeDir) + "/chat_export.md" : "./chat_export.md";
            }
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

void TerminalUI::handleSystemMessageInput(int key) {
    switch (key) {
        case '\n': // Enter key
            chatSession->setSystemMessage(inputBuffer);
            setStatusMessage("System message updated");
            currentScreen = Screen::MAIN_MENU;
            selectedOption = 0;
            break;
        case 27: // Escape key
            currentScreen = Screen::MAIN_MENU;
            selectedOption = 0;
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

void TerminalUI::setStatusMessage(const std::string& message) {
    statusMessage = message;
}

void TerminalUI::clearStatusMessage() {
    statusMessage.clear();
}

std::string TerminalUI::getProviderName(Provider provider) {
    return "Google";
}

void TerminalUI::refreshChatDisplay() {
    drawScreen();
}

void TerminalUI::sendChatMessage(const std::string& message) {
    try {
        // Set a status message to show we're processing
        setStatusMessage("Sending message to " + getProviderName(configManager->getSelectedProvider()) + "...");
        refreshChatDisplay();

        // Send the message with a safe callback
        chatSession->sendMessage(message, [this](const std::string& response, bool success) {
            try {
                if (!success) {
                    setStatusMessage("Error: " + response);
                } else {
                    clearStatusMessage();
                }

                refreshChatDisplay();
            } catch (const std::exception& e) {
                // Handle any exceptions in the callback
                setStatusMessage("Error in UI callback: " + std::string(e.what()));
                refreshChatDisplay();
            }
        });
    } catch (const std::exception& e) {
        // Handle any exceptions in the method
        setStatusMessage("Error sending message: " + std::string(e.what()));
        refreshChatDisplay();
    }
}

void TerminalUI::clearInputBuffer() {
    inputBuffer.clear();
}

void TerminalUI::appendToInputBuffer(int key) {
    inputBuffer += static_cast<char>(key);
}

void TerminalUI::backspaceInputBuffer() {
    if (!inputBuffer.empty()) {
        inputBuffer.pop_back();
    }
}

} // namespace libertymind
