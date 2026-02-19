#include "http_client.h"
#include <curl/curl.h>
#include <iostream>
#include <cstring>

/**
 * @brief Callback for writing downloaded data
 */
size_t HttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(userp);
    
    uint8_t* data = static_cast<uint8_t*>(contents);
    buffer->insert(buffer->end(), data, data + totalSize);
    
    return totalSize;
}

/**
 * @brief Callback for download progress
 */
int HttpClient::ProgressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    if (clientp) {
        auto callback = static_cast<std::function<void(size_t, size_t)>*>(clientp);
        if (*callback) {
            (*callback)(static_cast<size_t>(dlnow), static_cast<size_t>(dltotal));
        }
    }
    return 0;
}

/**
 * @brief Download binary data from URL
 */
int HttpClient::Download(const std::string& url, std::vector<uint8_t>& buffer, std::string& errorMsg)
{
    return DownloadWithProgress(url, buffer, errorMsg, nullptr);
}

/**
 * @brief Download binary data from URL with progress callback
 */
int HttpClient::DownloadWithProgress(
    const std::string& url,
    std::vector<uint8_t>& buffer,
    std::string& errorMsg,
    std::function<void(size_t, size_t)> progressCallback)
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        errorMsg = "Failed to initialize CURL";
        return -1;
    }
    
    buffer.clear();
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    if (progressCallback) {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &progressCallback);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    
    if (res != CURLE_OK) {
        errorMsg = curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        return -1;
    }
    
    curl_easy_cleanup(curl);
    return static_cast<int>(httpCode);
}

/**
 * @brief Download text content (for manifests)
 */
int HttpClient::DownloadText(const std::string& url, std::string& content, std::string& errorMsg)
{
    std::vector<uint8_t> buffer;
    int httpCode = Download(url, buffer, errorMsg);
    
    if (httpCode == 200 && !buffer.empty()) {
        content.assign(buffer.begin(), buffer.end());
    }
    
    return httpCode;
}
