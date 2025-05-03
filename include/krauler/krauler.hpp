#pragma once
#include "krauler/config.hpp"
#include "krauler/html_parser.hpp"
#include "krauler/http_client.hpp"

#include <curl/curl.h>
#include <gumbo.h>
#include <set>

namespace krauler {
// Write callback for libcurl
class Krauler {
  public:
    // Constructor
    Krauler(const Config& config) : config_(config) {
        // Initialize the crawler with the provided configuration
        spdlog::info("Krauler initialized with URL: {}", config_.url);
    }

    // Start crawling
    void start();

  private:
    Config config_; // Configuration object
};
} // namespace krauler