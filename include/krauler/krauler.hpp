#pragma once
#include "krauler/config.hpp"
#include "krauler/filter.hpp"
#include "krauler/html_parser.hpp"
#include "krauler/http_client.hpp"
#include "krauler/robots.hpp"

#include <boost/lockfree/queue.hpp>
#include <curl/curl.h>
#include <gumbo.h>
#include <queue>
#include <set>

namespace krauler {

struct UrlJob {
    char url[256]; // 固定长度字符数组，不能用 std::string

    UrlJob() = default;

    UrlJob(const char* str) {
        strncpy(url, str, sizeof(url));
        url[sizeof(url) - 1] = '\0'; // 确保 null terminator
    }

    // 不允许复杂成员：不能用 std::string
};

struct HtmlJob {
    char url[256];   // 固定长度字符数组，不能用 std::string
    char html[4096]; // 固定长度字符数组，不能用 std::string

    HtmlJob() = default;

    HtmlJob(const char* str, const char* html_str) {
        strncpy(url, str, sizeof(url));
        url[sizeof(url) - 1] = '\0'; // 确保 null terminator
        strncpy(html, html_str, sizeof(html));
        html[sizeof(html) - 1] = '\0'; // 确保 null terminator
    }

    // 不允许复杂成员：不能用 std::string
};

class Krauler {
  public:
    // Constructor
    Krauler(const Config& config) : config_(config) {
        // Initialize the crawler with the provided configuration
        spdlog::info("Krauler initialized with URL: {}", config_.url);
    }

    // Start crawling
    void run();

  private:
    Config config_; // Configuration object

    std::unordered_set<std::string> visited_urls_; // Set of visited URLs
    // std::unordered_set<std::string> extracted_urls_; // Set of extracted URLs
    std::queue<std::string>         url_queue_;  // Queue for URLs to crawl
    std::unordered_set<std::string> saved_urls_; // Set of saved URLs

    // std::unordered_set<std::string> visited_urls_; // Set of visited URLs

    // boost::lockfree::queue<UrlJob>  url_queue_;       // Queue for URLs to crawl
    // boost::lockfree::queue<HtmlJob> html_queue_;      // Queue for URLs to crawl
    // boost::lockfree::queue<UrlJob>  extracted_queue_; // Queue for links to crawl

    void save_html(const std::string& url, const std::string& html);
};

} // namespace krauler