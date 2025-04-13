#include "config_manager.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace libertymind {

ConfigManager::ConfigManager() : selectedProvider(Provider::GOOGLE), selectedModel("gemini-2.0-flash-lite") {
    initConfigPath();
    loadConfig();
}

void ConfigManager::initConfigPath() {
    // Create config directory in user's home directory
    const char* homeDir = getenv("HOME");
    if (homeDir) {
        configPath = fs::path(homeDir) / ".libertymind" / "config.json";
        fs::create_directories(configPath.parent_path());
    } else {
        // Fallback to current directory if HOME is not available
        configPath = fs::path(".libertymind") / "config.json";
        fs::create_directories(configPath.parent_path());
    }
}

bool ConfigManager::setApiKey(Provider provider, const std::string& key) {
    apiKeys[provider] = key;
    return saveConfig();
}

std::optional<std::string> ConfigManager::getApiKey(Provider provider) const {
    auto it = apiKeys.find(provider);
    if (it != apiKeys.end()) {
        return it->second;
    }
    return std::nullopt;
}

void ConfigManager::setSelectedProvider(Provider provider) {
    selectedProvider = provider;
    saveConfig();
}

Provider ConfigManager::getSelectedProvider() const {
    return selectedProvider;
}

void ConfigManager::setSelectedModel(const std::string& model) {
    selectedModel = model;
    saveConfig();
}

std::string ConfigManager::getSelectedModel() const {
    return selectedModel;
}

bool ConfigManager::saveConfig() const {
    try {
        json config;

        // Save provider and model selection
        config["selected_provider"] = providerToString(selectedProvider);
        config["selected_model"] = selectedModel;

        // Save API keys (in a real app, these should be encrypted)
        json keys;
        for (const auto& [provider, key] : apiKeys) {
            std::string encryptedKey = key;
            encryptApiKey(encryptedKey);
            keys[providerToString(provider)] = encryptedKey;
        }
        config["api_keys"] = keys;

        // Write to file
        std::ofstream file(configPath);
        if (!file.is_open()) {
            return false;
        }
        file << config.dump(4);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving config: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigManager::loadConfig() {
    if (!fs::exists(configPath)) {
        return false;
    }

    try {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            return false;
        }

        json config = json::parse(file);

        // Load provider and model selection
        if (config.contains("selected_provider")) {
            selectedProvider = stringToProvider(config["selected_provider"]);
        }

        if (config.contains("selected_model")) {
            selectedModel = config["selected_model"];
        }

        // Load API keys
        if (config.contains("api_keys") && config["api_keys"].is_object()) {
            for (auto& [providerStr, keyValue] : config["api_keys"].items()) {
                Provider provider = stringToProvider(providerStr);
                std::string key = keyValue;
                decryptApiKey(key);
                apiKeys[provider] = key;
            }
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        return false;
    }
}

std::string ConfigManager::providerToString(Provider provider) {
    return "google";
}

Provider ConfigManager::stringToProvider(const std::string& providerStr) {
    return Provider::GOOGLE;
}

// Simple XOR encryption for demonstration purposes
// In a real application, use a proper encryption library
void ConfigManager::encryptApiKey(std::string& key) const {
    const char xorKey = 0x42; // Simple XOR key
    for (char& c : key) {
        c ^= xorKey;
    }
}

void ConfigManager::decryptApiKey(std::string& key) const {
    // XOR encryption is symmetric, so decryption is the same as encryption
    encryptApiKey(key);
}

} // namespace libertymind
