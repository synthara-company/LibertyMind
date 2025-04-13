#include "chat_session.h"

namespace libertymind {

ChatSession::ChatSession(std::shared_ptr<ConfigManager> configManager)
    : configManager(configManager), systemMessage("You are Synthara, a helpful and intelligent assistant.") {
    // Add system message to history
    history.push_back({"system", systemMessage});
}

void ChatSession::sendMessage(const std::string& message, ChatCallback callback) {
    try {
        // Create a safe callback wrapper
        auto safeCallback = [callback](const std::string& response, bool success) {
            try {
                callback(response, success);
            } catch (const std::exception& e) {
                try {
                    callback("Critical error in callback: " + std::string(e.what()), false);
                } catch (...) {
                    // If even this fails, we can't do much more
                }
            } catch (...) {
                try {
                    callback("Unknown critical error in callback", false);
                } catch (...) {
                    // Last resort
                }
            }
        };

        // Add user message to history
        try {
            history.push_back({"user", message});
        } catch (const std::exception& e) {
            safeCallback("Error adding message to history: " + std::string(e.what()), false);
            return;
        }

        // Create API client
        std::unique_ptr<ApiClient> client;
        try {
            client = createClient();
            if (!client) {
                safeCallback("Error: Failed to create API client. Please check your API key.", false);
                return;
            }
        } catch (const std::exception& e) {
            safeCallback("Error creating API client: " + std::string(e.what()), false);
            return;
        }

        // Get selected model
        std::string model;
        try {
            model = configManager->getSelectedModel();
            if (model.empty()) {
                safeCallback("Error: No model selected", false);
                return;
            }
        } catch (const std::exception& e) {
            safeCallback("Error getting selected model: " + std::string(e.what()), false);
            return;
        }

        // Send request to API with a safe response handler
        try {
            client->sendChatCompletion(history, model, [this, safeCallback](const std::string& response, bool success) {
                try {
                    if (success && !response.empty()) {
                        try {
                            // Add assistant response to history
                            history.push_back({"assistant", response});
                        } catch (const std::exception& e) {
                            safeCallback("Error adding response to history: " + std::string(e.what()), false);
                            return;
                        }
                    }

                    // Call the callback with the response
                    safeCallback(response, success);
                } catch (const std::exception& e) {
                    // Handle any exceptions in the callback
                    safeCallback("Error in response handling: " + std::string(e.what()), false);
                } catch (...) {
                    safeCallback("Unknown error in response handling", false);
                }
            });
        } catch (const std::exception& e) {
            // Handle any exceptions in the API call
            safeCallback("Error sending message: " + std::string(e.what()), false);
        } catch (...) {
            safeCallback("Unknown error sending message", false);
        }
    } catch (const std::exception& e) {
        try {
            callback("Fatal error in message sending: " + std::string(e.what()), false);
        } catch (...) {
            // Last resort, can't do much if even this fails
        }
    } catch (...) {
        try {
            callback("Unknown fatal error in message sending", false);
        } catch (...) {
            // Last resort
        }
    }
}

void ChatSession::clearHistory() {
    history.clear();

    // Re-add system message
    history.push_back({"system", systemMessage});
}

const std::vector<Message>& ChatSession::getHistory() const {
    return history;
}

void ChatSession::setSystemMessage(const std::string& message) {
    systemMessage = message;

    // Update system message in history
    if (!history.empty() && history[0].role == "system") {
        history[0].content = message;
    } else {
        history.insert(history.begin(), {"system", message});
    }
}

std::string ChatSession::getSystemMessage() const {
    return systemMessage;
}

std::unique_ptr<ApiClient> ChatSession::createClient() const {
    Provider provider = configManager->getSelectedProvider();
    auto apiKey = configManager->getApiKey(provider);

    if (!apiKey) {
        return nullptr;
    }

    return createApiClient(provider, *apiKey);
}

} // namespace libertymind
