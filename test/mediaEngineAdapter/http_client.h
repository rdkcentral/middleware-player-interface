#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <curl/curl.h>

/**
 * @brief Simple HTTP client for downloading manifests and fragments
 */
class HttpClient
{
public:
    /**
     * @brief Download a file from URL
     * @param url URL to download
     * @param buffer Output buffer to store downloaded data
     * @param errorMsg Output error message if any
     * @return HTTP status code (200 = success)
     */
    static int Download(const std::string& url, std::vector<uint8_t>& buffer, std::string& errorMsg);
    
    /**
     * @brief Download a file from URL with progress callback
     * @param url URL to download
     * @param buffer Output buffer to store downloaded data
     * @param errorMsg Output error message if any
     * @param progressCallback Callback for download progress (bytesDownloaded, totalBytes)
     * @return HTTP status code (200 = success)
     */
    static int DownloadWithProgress(
        const std::string& url,
        std::vector<uint8_t>& buffer,
        std::string& errorMsg,
        std::function<void(size_t, size_t)> progressCallback = nullptr
    );
    
    /**
     * @brief Download text content (for manifests)
     * @param url URL to download
     * @param content Output string to store downloaded text
     * @param errorMsg Output error message if any
     * @return HTTP status code (200 = success)
     */
    static int DownloadText(const std::string& url, std::string& content, std::string& errorMsg);

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static int ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
};

#endif // HTTP_CLIENT_H
