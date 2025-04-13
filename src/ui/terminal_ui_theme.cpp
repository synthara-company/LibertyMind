#include "terminal_ui.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace libertymind {

void TerminalUI::initializeColorPairs() {
    // Initialize color pairs based on the current theme
    init_pair(1, currentTheme.headerFg, currentTheme.headerBg);    // Header
    init_pair(2, currentTheme.selectedFg, currentTheme.selectedBg); // Selected option
    init_pair(3, currentTheme.userMsgFg, COLOR_BLACK);             // User message
    init_pair(4, currentTheme.assistantMsgFg, COLOR_BLACK);        // Assistant message
    init_pair(5, currentTheme.errorMsgFg, COLOR_BLACK);            // Error message
}

bool TerminalUI::saveTheme() const {
    try {
        // Create theme directory in user's home directory
        const char* homeDir = getenv("HOME");
        if (!homeDir) {
            return false;
        }
        
        fs::path themePath = fs::path(homeDir) / ".libertymind" / "theme.json";
        fs::create_directories(themePath.parent_path());
        
        // Create JSON object with theme settings
        json themeJson = {
            {"headerFg", currentTheme.headerFg},
            {"headerBg", currentTheme.headerBg},
            {"selectedFg", currentTheme.selectedFg},
            {"selectedBg", currentTheme.selectedBg},
            {"userMsgFg", currentTheme.userMsgFg},
            {"assistantMsgFg", currentTheme.assistantMsgFg},
            {"errorMsgFg", currentTheme.errorMsgFg}
        };
        
        // Write to file
        std::ofstream file(themePath);
        if (!file.is_open()) {
            return false;
        }
        file << themeJson.dump(4);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool TerminalUI::loadTheme() {
    try {
        // Get theme file path
        const char* homeDir = getenv("HOME");
        if (!homeDir) {
            return false;
        }
        
        fs::path themePath = fs::path(homeDir) / ".libertymind" / "theme.json";
        if (!fs::exists(themePath)) {
            return false;
        }
        
        // Read theme file
        std::ifstream file(themePath);
        if (!file.is_open()) {
            return false;
        }
        
        json themeJson = json::parse(file);
        
        // Load theme settings
        if (themeJson.contains("headerFg")) {
            currentTheme.headerFg = themeJson["headerFg"];
        }
        if (themeJson.contains("headerBg")) {
            currentTheme.headerBg = themeJson["headerBg"];
        }
        if (themeJson.contains("selectedFg")) {
            currentTheme.selectedFg = themeJson["selectedFg"];
        }
        if (themeJson.contains("selectedBg")) {
            currentTheme.selectedBg = themeJson["selectedBg"];
        }
        if (themeJson.contains("userMsgFg")) {
            currentTheme.userMsgFg = themeJson["userMsgFg"];
        }
        if (themeJson.contains("assistantMsgFg")) {
            currentTheme.assistantMsgFg = themeJson["assistantMsgFg"];
        }
        if (themeJson.contains("errorMsgFg")) {
            currentTheme.errorMsgFg = themeJson["errorMsgFg"];
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

void TerminalUI::applyTheme() {
    // Re-initialize color pairs with the current theme
    initializeColorPairs();
    
    // Save the theme to file
    saveTheme();
}

void TerminalUI::drawThemeCustomization() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Customize Theme");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);
    
    // Theme options
    const std::vector<std::string> options = {
        "Header Text Color",
        "Header Background Color",
        "Selected Item Text Color",
        "Selected Item Background Color",
        "User Message Color",
        "Assistant Message Color",
        "Error Message Color",
        "Save Theme",
        "Reset to Default",
        "Back to Main Menu"
    };
    
    // Draw theme options
    for (size_t i = 0; i < options.size(); ++i) {
        if (static_cast<int>(i) == selectedOption) {
            wattron(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }
        
        mvwprintw(mainWindow, i + 2, 2, "%s", options[i].c_str());
        
        // Show current color for color options
        if (i < 7) {
            int colorValue;
            switch (i) {
                case 0: colorValue = currentTheme.headerFg; break;
                case 1: colorValue = currentTheme.headerBg; break;
                case 2: colorValue = currentTheme.selectedFg; break;
                case 3: colorValue = currentTheme.selectedBg; break;
                case 4: colorValue = currentTheme.userMsgFg; break;
                case 5: colorValue = currentTheme.assistantMsgFg; break;
                case 6: colorValue = currentTheme.errorMsgFg; break;
                default: colorValue = COLOR_WHITE;
            }
            
            std::string colorName;
            switch (colorValue) {
                case COLOR_BLACK: colorName = "Black"; break;
                case COLOR_RED: colorName = "Red"; break;
                case COLOR_GREEN: colorName = "Green"; break;
                case COLOR_YELLOW: colorName = "Yellow"; break;
                case COLOR_BLUE: colorName = "Blue"; break;
                case COLOR_MAGENTA: colorName = "Magenta"; break;
                case COLOR_CYAN: colorName = "Cyan"; break;
                case COLOR_WHITE: colorName = "White"; break;
                default: colorName = "Unknown";
            }
            
            mvwprintw(mainWindow, i + 2, 30, "Current: %s", colorName.c_str());
        }
        
        if (static_cast<int>(i) == selectedOption) {
            wattroff(mainWindow, COLOR_PAIR(2) | A_BOLD);
        }
    }
    
    // Draw preview section
    int y = options.size() + 3;
    mvwprintw(mainWindow, y++, 2, "Preview:");
    
    // Header preview
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, y++, 4, "Header Text");
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);
    
    // Selected item preview
    wattron(mainWindow, COLOR_PAIR(2) | A_BOLD);
    mvwprintw(mainWindow, y++, 4, "Selected Item");
    wattroff(mainWindow, COLOR_PAIR(2) | A_BOLD);
    
    // User message preview
    wattron(mainWindow, COLOR_PAIR(3) | A_BOLD);
    mvwprintw(mainWindow, y++, 4, "User Message");
    wattroff(mainWindow, COLOR_PAIR(3) | A_BOLD);
    
    // Assistant message preview
    wattron(mainWindow, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(mainWindow, y++, 4, "Assistant Message");
    wattroff(mainWindow, COLOR_PAIR(4) | A_BOLD);
    
    // Error message preview
    wattron(mainWindow, COLOR_PAIR(5) | A_BOLD);
    mvwprintw(mainWindow, y++, 4, "Error Message");
    wattroff(mainWindow, COLOR_PAIR(5) | A_BOLD);
    
    // Draw instructions
    y += 2;
    mvwprintw(mainWindow, y++, 2, "Instructions:");
    mvwprintw(mainWindow, y++, 4, "Use arrow keys to navigate");
    mvwprintw(mainWindow, y++, 4, "Press Enter to select/cycle through colors");
    mvwprintw(mainWindow, y++, 4, "Press Escape to go back without saving");
}

void TerminalUI::handleThemeCustomizationInput(int key) {
    const int numOptions = 10; // Total number of options in the theme customization screen
    
    switch (key) {
        case KEY_UP:
            selectedOption = std::max(0, selectedOption - 1);
            break;
        case KEY_DOWN:
            selectedOption = std::min(numOptions - 1, selectedOption + 1);
            break;
        case '\n': // Enter key
            if (selectedOption < 7) {
                // Color options - cycle through available colors
                int* colorPtr = nullptr;
                switch (selectedOption) {
                    case 0: colorPtr = &currentTheme.headerFg; break;
                    case 1: colorPtr = &currentTheme.headerBg; break;
                    case 2: colorPtr = &currentTheme.selectedFg; break;
                    case 3: colorPtr = &currentTheme.selectedBg; break;
                    case 4: colorPtr = &currentTheme.userMsgFg; break;
                    case 5: colorPtr = &currentTheme.assistantMsgFg; break;
                    case 6: colorPtr = &currentTheme.errorMsgFg; break;
                }
                
                if (colorPtr) {
                    // Cycle through colors (0-7)
                    *colorPtr = (*colorPtr + 1) % 8;
                    applyTheme();
                }
            } else if (selectedOption == 7) {
                // Save Theme
                if (saveTheme()) {
                    setStatusMessage("Theme saved successfully");
                } else {
                    setStatusMessage("Error: Failed to save theme");
                }
            } else if (selectedOption == 8) {
                // Reset to Default
                currentTheme = Theme(); // Reset to default theme
                applyTheme();
                setStatusMessage("Theme reset to default");
            } else if (selectedOption == 9) {
                // Back to Main Menu
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

} // namespace libertymind
