#pragma once

#include "config_manager.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace libertymind {

struct ModelInfo {
    std::string id;
    std::string name;
    std::string description;
};

class ModelRegistry {
public:
    ModelRegistry();
    ~ModelRegistry() = default;
    
    // Get all models for a specific provider
    std::vector<ModelInfo> getModelsForProvider(Provider provider) const;
    
    // Get all available providers
    std::vector<Provider> getAvailableProviders() const;
    
    // Check if a model is valid for a provider
    bool isValidModel(Provider provider, const std::string& modelId) const;
    
    // Get model info by ID
    std::optional<ModelInfo> getModelInfo(Provider provider, const std::string& modelId) const;

private:
    std::unordered_map<Provider, std::vector<ModelInfo>> models;
    
    void initializeModels();
};

} // namespace libertymind
