#include "api_client.h"
#include <curl/curl.h>
#include <iostream>

namespace libertymind {

ApiClient::ApiClient(const std::string& apiKey) : apiKey(apiKey) {
    // Initialize curl globally (should be called once in the application)
    static bool curlInitialized = false;
    if (!curlInitialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curlInitialized = true;
    }
}

size_t ApiClient::writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

bool ApiClient::makePostRequest(
    const std::string& url,
    const nlohmann::json& payload,
    std::string& response,
    const std::vector<std::string>& headers
) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl" << std::endl;
        return false;
    }
    
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set POST method
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    
    // Set request body
    std::string payloadStr = payload.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadStr.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payloadStr.size());
    
    // Set headers
    struct curl_slist* headerList = nullptr;
    headerList = curl_slist_append(headerList, "Content-Type: application/json");
    
    for (const auto& header : headers) {
        headerList = curl_slist_append(headerList, header.c_str());
    }
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    
    // Set write callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    // Perform request
    CURLcode res = curl_easy_perform(curl);
    
    // Check for errors
    bool success = (res == CURLE_OK);
    if (!success) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    
    // Cleanup
    curl_slist_free_all(headerList);
    curl_easy_cleanup(curl);
    
    return success;
}

} // namespace libertymind
