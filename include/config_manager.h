#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <optional>

namespace libertymind {

enum class Provider {
    GOOGLE
};

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager() = default;

    // API key management
    bool setApiKey(Provider provider, const std::string& key);
    std::optional<std::string> getApiKey(Provider provider) const;

    // Provider and model selection
    void setSelectedProvider(Provider provider);
    Provider getSelectedProvider() const;

    void setSelectedModel(const std::string& model);
    std::string getSelectedModel() const;

    // Save and load configuration
    bool saveConfig() const;
    bool loadConfig();

    // Convert Provider enum to string and back
    static std::string providerToString(Provider provider);
    static Provider stringToProvider(const std::string& providerStr);

private:
    std::unordered_map<Provider, std::string> apiKeys;
    Provider selectedProvider;
    std::string selectedModel;
    std::filesystem::path configPath;

    void initConfigPath();
    void encryptApiKey(std::string& key) const;
    void decryptApiKey(std::string& key) const;
};

} // namespace libertymind
