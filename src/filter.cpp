#include "krauler/filter.hpp"

namespace krauler {

Filter::Filter(const Config& config, const Robotstxt& robotstxt,
               const std::unordered_set<std::string>& visited_urls)
    : config_(config), robotstxt_(robotstxt), visited_urls_(visited_urls) {
    // find rules with user agaent name
    bool matched = false;

    // Ignore file suffix
    std::unordered_set<std::string> ignore_suffixes = {".jpg", ".jpeg", ".png", ".gif",   ".css",
                                                       ".js",  ".ico",  ".svg", ".woff2", ".pdf"};
    for (const auto& suffix : ignore_suffixes) {
        regexes_.insert(config_.url + ".*" + suffix);
    }

    // Handle robots.txt rules
    for (const auto& [user_agent, paths] : robotstxt_.disallowed_) {
        std::string agent_pattern = robots2stdregex(user_agent);
        spdlog::debug("Filter: PATTERN: \"{}\"", agent_pattern);

        std::regex agent_regex(agent_pattern);
        if (std::regex_match(config_.user_agent, agent_regex)) {
            matched = true;
            spdlog::debug("Filter: found rules in \"{}\" for user agent \"{}\"", agent_pattern,
                          config_.user_agent);
            for (const auto& path : paths) {
                std::string full_regex = url_join(config_.url, robots2stdregex(path));
                regexes_.insert(full_regex);
                spdlog::debug("Filter: added regex \"{}\"", full_regex);
            }
        }
    }
    if (!matched) {
        spdlog::warn("Filter: no rules found for user agent \"{}\"", config_.user_agent);
    }
}
std::string Filter::robots2stdregex(const std::string& rule) {
    std::string regex_pattern;

    for (size_t i = 0; i < rule.length(); ++i) {
        char c = rule[i];

        switch (c) {
            case '*':
                // Wildcard: match any sequence of characters
                regex_pattern += ".*";
                break;

            case '$':
                // End anchor: match end of string
                regex_pattern += "$";
                break;

            // Escape special regex characters
            case '.':
            case '?':
            case '+':
            case '{':
            case '}':
            case '[':
            case ']':
            case '(':
            case ')':
            case '|':
            case '\\':
            case '^':
                regex_pattern += '\\';
                regex_pattern += c;
                break;

            default:
                // Regular character
                regex_pattern += c;
                break;
        }
    }

    // If the pattern doesn't end with $, it can match any suffix
    if (!regex_pattern.empty() && regex_pattern.back() != '$') {
        // Add implicit ".*" at the end to match the robots.txt behavior
        // where patterns without $ match any URL that starts with the pattern
        regex_pattern += ".*";
    }

    return regex_pattern;
}
bool Filter::is_allowed(const std::string& url) const {
    // check if the URL is in the domain
    if (url.find(config_.url) == std::string::npos) {
        spdlog::debug("Filter: \"{}\" not in the domain \"{}\"", url, config_.url);
        return false;
    }
    // Check if the URL is in the visited set
    if (visited_urls_.find(url) != visited_urls_.end()) {
        spdlog::debug("Filter: \"{}\" visited", url);
        return false;
    }
    // regex check
    for (const auto& regex : regexes_) {
        std::regex pattern(regex);
        if (std::regex_match(url, pattern)) {
            spdlog::debug("Filter: \"{}\" matched regex \"{}\"", url, regex);
            return false;
        }
    }

    return true;
}
} // namespace krauler