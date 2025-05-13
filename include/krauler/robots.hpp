#pragma once
#include "spdlog/spdlog.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace krauler {
class Robotstxt {
  public:
    Robotstxt(const std::string& url);
    void parse_robots(const std::string& robots_txt);
    void parse_sitemap();
    friend class Filter;

  private:
    std::string                                                      base_url_;
    std::unordered_set<std::string>                                  sitemaps_;
    std::unordered_map<std::string, std::unordered_set<std::string>> disallowed_;
};
} // namespace krauler