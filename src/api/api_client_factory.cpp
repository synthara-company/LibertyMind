#include "api_client.h"
#include "google_client.h"

namespace libertymind {

std::unique_ptr<ApiClient> createApiClient(Provider provider, const std::string& apiKey) {
    return std::make_unique<GoogleClient>(apiKey);
}

} // namespace libertymind
