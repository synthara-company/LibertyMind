#include "google_client.h"
#include <iostream>
#include <thread>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace libertymind {

GoogleClient::GoogleClient(const std::string& apiKey) : ApiClient(apiKey) {}

std::string GoogleClient::getBaseUrl() const {
    return "https://generativelanguage.googleapis.com/v1";
}

// Simple callback function for curl
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch(std::bad_alloc& e) {
        return 0;
    }
}

void GoogleClient::sendChatCompletion(
    const std::vector<Message>& messages,
    const std::string& model,
    CompletionCallback callback
) {
    // Uncommented and fixed implementation to use the actual Google API

    // Extract the last user message for simplicity
    std::string userMessage = "Hello";
    for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
        if (it->role == "user") {
            userMessage = it->content;
            break;
        }
    }

    // Create a thread to handle the API request
    std::thread([userMessage, model, apiKey=this->apiKey, callback]() {
        // Initialize curl
        CURL* curl = curl_easy_init();
        if (!curl) {
            callback("Error: Failed to initialize curl", false);
            return;
        }

        // Prepare URL
        std::string url = "https://generativelanguage.googleapis.com/v1/models/" +
                          model + ":generateContent?key=" + apiKey;

        // Prepare JSON payload with proper escaping
        std::string escapedMessage = userMessage;
        // Replace backslashes with double backslashes
        size_t pos = 0;
        while ((pos = escapedMessage.find("\\", pos)) != std::string::npos) {
            escapedMessage.replace(pos, 1, "\\\\");
            pos += 2;
        }
        // Replace double quotes with escaped quotes
        pos = 0;
        while ((pos = escapedMessage.find("\"", pos)) != std::string::npos) {
            escapedMessage.replace(pos, 1, "\\\"");
            pos += 2;
        }
        // Replace newlines with \n
        pos = 0;
        while ((pos = escapedMessage.find("\n", pos)) != std::string::npos) {
            escapedMessage.replace(pos, 1, "\\n");
            pos += 2;
        }

        // Create the JSON payload
        std::string jsonPayload = "{\"contents\":[{\"role\":\"user\",\"parts\":[{\"text\":\"" +
                                 escapedMessage + "\"}]}],\"generationConfig\":{\"temperature\":0.7,\"maxOutputTokens\":2000}}";

        // Response buffer
        std::string responseText;

        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseText);

        // Set headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            callback(std::string("Error: ") + curl_easy_strerror(res), false);
        } else {
            // Parse the JSON response
            try {
                // Use nlohmann/json to parse the response
                nlohmann::json responseJson = nlohmann::json::parse(responseText);

                // Check if we have candidates
                if (responseJson.contains("candidates") &&
                    responseJson["candidates"].is_array() &&
                    !responseJson["candidates"].empty()) {

                    // Get the first candidate
                    const auto& candidate = responseJson["candidates"][0];

                    // Check if it has content and parts
                    if (candidate.contains("content") &&
                        candidate["content"].contains("parts") &&
                        candidate["content"]["parts"].is_array() &&
                        !candidate["content"]["parts"].empty()) {

                        // Get the text from the first part
                        const auto& part = candidate["content"]["parts"][0];
                        if (part.contains("text")) {
                            std::string text = part["text"];
                            callback(text, true);
                        } else {
                            callback("Error: No text found in response", false);
                        }
                    } else {
                        callback("Error: Invalid content format in response", false);
                    }
                } else if (responseJson.contains("error")) {
                    // Handle API error
                    std::string errorMessage = "API Error";
                    if (responseJson["error"].contains("message")) {
                        errorMessage = responseJson["error"]["message"];
                    }
                    callback("Error: " + errorMessage, false);
                } else {
                    // Fallback error message with partial response
                    callback("Error: Unexpected response format. Response: " +
                             responseText.substr(0, 100) + "...", false);
                }
            } catch (const std::exception& e) {
                // JSON parsing error
                callback("Error parsing response: " + std::string(e.what()) +
                         "\nResponse: " + responseText.substr(0, 100) + "...", false);
            }
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }).detach();

}

bool GoogleClient::validateApiKey() {
    // For Google API keys, we'll be more lenient with validation
    // Just check if it's not empty
    if (apiKey.empty()) {
        return false;
    }

    // Print a helpful message about Google API keys
    std::cout << "\nNote: For Google Gemini API, you need an API key from Google AI Studio." << std::endl;
    std::cout << "Visit: https://aistudio.google.com/ to get your API key." << std::endl;

    std::cout << "\nSynthara supports these Gemini models:" << std::endl;
    std::cout << "- Gemini 2.0 Flash-Lite: Cost-effective model for simple tasks" << std::endl;
    std::cout << "- Gemini 1.5 Pro: Google's versatile model with long context" << std::endl;
    std::cout << "- Gemini 1.5 Flash: Faster, more efficient Google model" << std::endl;

    return true;
}

} // namespace libertymind
