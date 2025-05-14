#pragma once
#include "krauler/config.hpp"
#include "krauler/html_parser.hpp"
#include "krauler/robots.hpp"
#include "spdlog/spdlog.h"

#include <regex>
#include <string>
#include <unordered_set>

namespace krauler {

class Filter {
  public:
    Filter(const Config& config, const Robotstxt& robotstxt,
           const std::unordered_set<std::string>& visited_urls);
    // Check if the URL is allowed to be crawled
    bool is_allowed(const std::string& url) const;

  private:
    const Config&                          config_;       // Configuration object
    const Robotstxt&                       robotstxt_;    // Robotstxt object
    const std::unordered_set<std::string>& visited_urls_; // Visited URLs
    std::unordered_set<std::string>        regexes_;      // Regex patterns>

    std::string robots2stdregex(const std::string& rule);
};
} // namespace krauler