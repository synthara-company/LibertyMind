#include "terminal_ui.h"
#include <string>
#include <vector>

namespace libertymind {

void TerminalUI::drawCompanyInfo() {
    // Draw header
    wattron(mainWindow, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(mainWindow, 0, 0, "Company Information");
    whline(mainWindow, ' ', getmaxx(mainWindow));
    wattroff(mainWindow, COLOR_PAIR(1) | A_BOLD);

    // Company information
    int y = 2;

    // Company name and logo (ASCII art)
    wattron(mainWindow, A_BOLD);
    mvwprintw(mainWindow, y++, 2, "LibertyMind Research & Software");
    wattroff(mainWindow, A_BOLD);
    y++;

    // ASCII art logo
    wattron(mainWindow, COLOR_PAIR(3));
    mvwprintw(mainWindow, y++, 2, "  _     _ _               _         __  __ _           _ ");
    mvwprintw(mainWindow, y++, 2, " | |   (_) |__   ___ _ __| |_ _   _|  \\/  (_)_ __   __| |");
    mvwprintw(mainWindow, y++, 2, " | |   | | '_ \\ / _ \\ '__| __| | | | |\\/| | | '_ \\ / _` |");
    mvwprintw(mainWindow, y++, 2, " | |___| | |_) |  __/ |  | |_| |_| | |  | | | | | | (_| |");
    mvwprintw(mainWindow, y++, 2, " |_____|_|_.__/ \\___|_|   \\__|\\__, |_|  |_|_|_| |_|\\__,_|");
    mvwprintw(mainWindow, y++, 2, "                              |___/                      ");
    wattroff(mainWindow, COLOR_PAIR(3));
    y++;

    // About section
    wattron(mainWindow, A_BOLD);
    mvwprintw(mainWindow, y++, 2, "About Us:");
    wattroff(mainWindow, A_BOLD);

    std::vector<std::string> aboutLines = {
        "LibertyMind is a research service provider and computer software company",
        "specializing in artificial intelligence and natural language processing.",
        "",
        "We develop cutting-edge tools and applications that leverage the power of",
        "language models to solve complex problems and enhance productivity.",
        "",
        "Our flagship product, Synthara, provides a streamlined terminal interface",
        "for interacting with state-of-the-art language models."
    };

    for (const auto& line : aboutLines) {
        mvwprintw(mainWindow, y++, 2, "%s", line.c_str());
    }
    y++;

    // Products section
    wattron(mainWindow, A_BOLD);
    mvwprintw(mainWindow, y++, 2, "Our Products:");
    wattroff(mainWindow, A_BOLD);

    std::vector<std::string> productLines = {
        "• Synthara - Terminal-based LLM interface",
        "• LibertyMind API - Enterprise AI integration services",
        "• ResearchAssist - AI-powered research tools"
    };

    for (const auto& line : productLines) {
        mvwprintw(mainWindow, y++, 2, "%s", line.c_str());
    }
    y++;

    // Contact information
    wattron(mainWindow, A_BOLD);
    mvwprintw(mainWindow, y++, 2, "Contact Information:");
    wattroff(mainWindow, A_BOLD);

    mvwprintw(mainWindow, y++, 2, "Website: www.libertymind.ai");
    mvwprintw(mainWindow, y++, 2, "Email: contact@libertymind.ai");
    mvwprintw(mainWindow, y++, 2, "GitHub: github.com/libertymind");
    mvwprintw(mainWindow, y++, 2, "Copyright © 2025 LibertyMind");
    y++;

    // Footer
    mvwprintw(mainWindow, y++, 2, "Press any key to return to the main menu");
}

void TerminalUI::handleCompanyInfoInput(int key) {
    // Any key returns to main menu
    currentScreen = Screen::MAIN_MENU;
    selectedOption = 0;
}

} // namespace libertymind
