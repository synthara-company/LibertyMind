#pragma once

#include "api_client.h"

namespace libertymind {

class GoogleClient : public ApiClient {
public:
    GoogleClient(const std::string& apiKey);
    ~GoogleClient() override = default;
    
    void sendChatCompletion(
        const std::vector<Message>& messages,
        const std::string& model,
        CompletionCallback callback
    ) override;
    
    bool validateApiKey() override;
    
    std::string getBaseUrl() const override;
};

} // namespace libertymind
