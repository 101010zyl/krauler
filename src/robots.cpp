#include "krauler/robots.hpp"

#include "krauler/http_client.hpp"

#include <sstream>

namespace krauler {
Robotstxt::Robotstxt(const std::string& url) : base_url_(url) {
    std::string robots_txt = fetch_url(url + "/robots.txt");
    spdlog::info("Fetching robots.txt from {}", url + "/robots.txt");
    // spdlog::info("Fetched {} bytes", robots_txt.size());
    // spdlog::info("robots.txt content: {}", robots_txt);
    try {
        parse_robots(robots_txt);
        parse_sitemap();
    } catch (const std::exception& e) {
        spdlog::error("Failed to parse robots.txt: {}", e.what());
    }
}

void Robotstxt::parse_robots(const std::string& robots_txt) {
    std::istringstream stream(robots_txt);
    std::string        line;
    std::string        user_agent;
    while (std::getline(stream, line)) {
        // spdlog::info("Parsing line: {}", line);
        if (line.find("User-agent:") == 0) {
            user_agent              = line.substr(11);
            disallowed_[user_agent] = {};
        } else if (line.find("Disallow:") == 0) {
            if (!user_agent.empty()) {
                std::string path = line.substr(9);
                disallowed_[user_agent].insert(path);
            }
        } else if (line.find("Sitemap:") == 0) {
            std::string sitemap_url = line.substr(8);
            sitemaps_.insert(sitemap_url);
        }
    }
    // print rules
    for (const auto& [user_agent, paths] : disallowed_) {
        spdlog::debug("User-agent: {}", user_agent);
        for (const auto& path : paths) {
            spdlog::debug("  Disallow: {}", path);
        }
    }
    for (const auto& sitemap : sitemaps_) {
        spdlog::debug("Sitemap: {}", sitemap);
    }
}
void Robotstxt::parse_sitemap() {
    for (const auto& sitemap : sitemaps_) {
        // std::string sitemap_content = fetch_url(sitemap);
        // spdlog::info("Fetched {} bytes from {}", sitemap_content.size(), sitemap);
        // Parse the sitemap content
    }
}

bool Robotstxt::can_fetch(const std::string& user_agent, const std::string& url) {
    // Check if the URL is disallowed for the given user agent
    auto rules = disallowed_.find(user_agent);
    if (rules == disallowed_.end()) {
        spdlog::info("User-agent {} not found in robots.txt", user_agent);
        return true; // No rules for this user agent, allow access
    }
    // Check if the URL matches any disallowed paths
    spdlog::info("Checking URL: {}", url);
    if (url.find(base_url_) != 0) {
        spdlog::info("URL {} does not start with base URL {}", url, base_url_);
        return true; // URL does not match base URL, allow access
    }
    return true; // URL is allowed
}

} // namespace krauler