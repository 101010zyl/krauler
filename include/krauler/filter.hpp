#pragma once
#include "krauler/config.hpp"
#include "krauler/robots.hpp"
#include "spdlog/spdlog.h"

#include <string>
#include <unordered_set>

namespace krauler {

class Filter {
  public:
    Filter(const Config& config, const Robotstxt& robotstxt,
           const std::unordered_set<std::string>& visited_urls)
        : config_(config), robotstxt_(robotstxt), visited_urls_(visited_urls) {}
    // Check if the URL is allowed to be crawled
    bool is_allowed(const std::string& url) const {
        // Check if the URL is in the disallowed list
        if (robotstxt_.disallowed_.count(url)) {
            spdlog::warn("URL {} is disallowed by robots.txt", url);
            return false;
        }
        // Check if the URL is in the visited set
        if (visited_urls_.count(url)) {
            spdlog::debug("URL {} has already been visited", url);
            return false;
        }
        return true;
    }

  private:
    const Config&                          config_;       // Configuration object
    const Robotstxt&                       robotstxt_;    // Robotstxt object
    const std::unordered_set<std::string>& visited_urls_; // Visited URLs
};
} // namespace krauler