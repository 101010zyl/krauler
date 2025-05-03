#pragma once
#include <string>

namespace krauler {

class Filter {
  public:
    Filter(const std::string& user_agent, const std::string& robots_txt)
        : user_agent_(user_agent), robots_txt_(robots_txt) {}

    bool is_allowed(const std::string& url) {
        // Implement the logic to check if the URL is allowed based on the robots.txt
        // For now, we will just return true for all URLs
        return true;
    }
    bool is_allowed(const std::string& url, const std::string& user_agent) {
        // Implement the logic to check if the URL is allowed based on the robots.txt
        // For now, we will just return true for all URLs
        return true;
    }
    bool is_allowed(const std::string& url, const std::string& user_agent,
                    const std::string& robots_txt) {
        // Implement the logic to check if the URL is allowed based on the robots.txt
        // For now, we will just return true for all URLs
        return true;
    }

  private:
    std::string user_agent_; // User agent string
    std::string robots_txt_; // Robots.txt content
};
} // namespace krauler