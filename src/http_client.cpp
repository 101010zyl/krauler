#include "krauler/http_client.hpp"

#include <curl/curl.h>
#include <stdexcept>

namespace krauler {

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t       totalBytes = size * nmemb;
    std::string* output     = static_cast<std::string*>(userp);
    output->append(static_cast<char*>(contents), totalBytes);
    return totalBytes;
}

std::string fetch_url(const std::string& url) {
    CURL*       curl;
    CURLcode    res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to initialize CURL");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        throw std::runtime_error(curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return response;
}

} // namespace krauler
