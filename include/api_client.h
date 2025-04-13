#pragma once

#include "config_manager.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

namespace libertymind {

struct Message {
    std::string role;
    std::string content;
};

using CompletionCallback = std::function<void(const std::string&, bool)>;

class ApiClient {
public:
    ApiClient(const std::string& apiKey);
    virtual ~ApiClient() = default;

    // Send a chat completion request
    virtual void sendChatCompletion(
        const std::vector<Message>& messages,
        const std::string& model,
        CompletionCallback callback
    ) = 0;

    // Check if API key is valid
    virtual bool validateApiKey() = 0;

    // Get the base URL for the API
    virtual std::string getBaseUrl() const = 0;

protected:
    std::string apiKey;

    // Helper method for making HTTP requests
    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data);

    // Make a POST request with JSON payload
    bool makePostRequest(
        const std::string& url,
        const nlohmann::json& payload,
        std::string& response,
        const std::vector<std::string>& headers = {}
    );
};

// Factory function to create the appropriate API client
std::unique_ptr<ApiClient> createApiClient(Provider provider, const std::string& apiKey);

} // namespace libertymind
