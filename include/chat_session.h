#pragma once

#include "api_client.h"
#include "config_manager.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace libertymind {

using ChatCallback = std::function<void(const std::string&, bool)>;

class ChatSession {
public:
    ChatSession(std::shared_ptr<ConfigManager> configManager);
    ~ChatSession() = default;
    
    // Send a message to the model
    void sendMessage(const std::string& message, ChatCallback callback);
    
    // Clear the conversation history
    void clearHistory();
    
    // Get the conversation history
    const std::vector<Message>& getHistory() const;
    
    // Set system message
    void setSystemMessage(const std::string& message);
    
    // Get system message
    std::string getSystemMessage() const;

private:
    std::shared_ptr<ConfigManager> configManager;
    std::vector<Message> history;
    std::string systemMessage;
    
    // Create a new API client based on the current configuration
    std::unique_ptr<ApiClient> createClient() const;
};

} // namespace libertymind
