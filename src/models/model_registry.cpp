#include "model_registry.h"

namespace libertymind {

ModelRegistry::ModelRegistry() {
    initializeModels();
}

void ModelRegistry::initializeModels() {
    // Google models
    models[Provider::GOOGLE] = {
        // Gemini 2.0 models
        {"gemini-2.0-flash-lite", "Gemini 2.0 Flash-Lite", "Cost-effective model for simple tasks"},

        // Gemini 1.5 models
        {"gemini-1.5-pro", "Gemini 1.5 Pro", "Google's versatile model with long context"},
        {"gemini-1.5-flash", "Gemini 1.5 Flash", "Faster, more efficient Google model"}
    };
}

std::vector<ModelInfo> ModelRegistry::getModelsForProvider(Provider provider) const {
    auto it = models.find(provider);
    if (it != models.end()) {
        return it->second;
    }
    return {};
}

std::vector<Provider> ModelRegistry::getAvailableProviders() const {
    std::vector<Provider> providers;
    for (const auto& [provider, _] : models) {
        providers.push_back(provider);
    }
    return providers;
}

bool ModelRegistry::isValidModel(Provider provider, const std::string& modelId) const {
    auto it = models.find(provider);
    if (it == models.end()) {
        return false;
    }

    for (const auto& model : it->second) {
        if (model.id == modelId) {
            return true;
        }
    }

    return false;
}

std::optional<ModelInfo> ModelRegistry::getModelInfo(Provider provider, const std::string& modelId) const {
    auto it = models.find(provider);
    if (it == models.end()) {
        return std::nullopt;
    }

    for (const auto& model : it->second) {
        if (model.id == modelId) {
            return model;
        }
    }

    return std::nullopt;
}

} // namespace libertymind
