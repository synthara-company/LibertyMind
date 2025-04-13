#include "terminal_ui.h"
#include <iostream>
#include <stdexcept>
#include <curl/curl.h>

int main() {
    try {
        // Initialize curl globally
        curl_global_init(CURL_GLOBAL_ALL);
        
        // Create and run the terminal UI
        libertymind::TerminalUI ui;
        ui.run();
        
        // Clean up curl
        curl_global_cleanup();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
