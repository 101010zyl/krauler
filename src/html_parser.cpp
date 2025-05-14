#include "krauler/html_parser.hpp"

#include <gumbo.h>
#include <algorithm>
#include <unordered_set>

namespace krauler {

static void search_links(GumboNode* node, std::set<std::string>& links) {
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    if (node->v.element.tag == GUMBO_TAG_A) {
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href && href->value) {
            links.insert(href->value);
        }
    }

    const GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_links(static_cast<GumboNode*>(children->data[i]), links);
    }
}

std::set<std::string> extract_links(const std::string& html) {
    GumboOutput*          output = gumbo_parse(html.c_str());
    std::set<std::string> links;
    search_links(output->root, links);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return links;
}

std::string url_join(const std::string& base, const std::string& path) {
    if (path.empty()) {
        return base;
    }

    std::string result = base;

    // Remove protocol if present (http:// or https://)
    if (result.starts_with("http://")) {
        result = result.substr(7);
    } else if (result.starts_with("https://")) {
        result = result.substr(8);
    }

    // 处理 base: 确保不以多个 / 结尾
    while (!result.empty() && result.back() == '/')
        result.pop_back();

    // 处理 path: 去除前导的多个 /
    size_t path_start = 0;
    while (path_start < path.size() && path[path_start] == '/')
        ++path_start;

    // If path is only slashes, return base
    if (path_start >= path.size()) {
        return result;
    }

    // 加入 /
    result += "/";
    result += path.substr(path_start);

    // Remove trailing slash (except for root path)
    if (result.length() > 1 && result.back() == '/') {
        result.pop_back();
    }

    return result;
}

std::string normalize_url(const std::string& hostname, const std::string& relative_url) {
    // First, remove fragment (anything after #) from the URL
    std::string url_without_fragment = relative_url;
    size_t      fragment_pos         = url_without_fragment.find('#');
    if (fragment_pos != std::string::npos) {
        url_without_fragment = url_without_fragment.substr(0, fragment_pos);
    }

    // Handle empty URL after fragment removal
    if (url_without_fragment.empty() || url_without_fragment == "/") {
        std::string result = hostname;
        // Remove protocol if present
        if (result.starts_with("http://")) {
            result = result.substr(7);
        } else if (result.starts_with("https://")) {
            result = result.substr(8);
        }
        // Remove trailing slash
        if (result.length() > 0 && result.back() == '/') {
            result.pop_back();
        }
        return result;
    }

    // Full URL (http:// or https://)
    if (url_without_fragment.starts_with("http://") ||
        url_without_fragment.starts_with("https://")) {
        // Remove protocol
        std::string result;
        if (url_without_fragment.starts_with("http://")) {
            result = url_without_fragment.substr(7);
        } else {
            result = url_without_fragment.substr(8);
        }
        // Remove trailing slash
        if (result.length() > 0 && result.back() == '/') {
            result.pop_back();
        }
        return result;
    }

    // Protocol-relative URL (//example.com/path)
    if (url_without_fragment.starts_with("//")) {
        // Remove the // prefix
        std::string result = url_without_fragment.substr(2);
        // Remove trailing slash
        if (result.length() > 0 && result.back() == '/') {
            result.pop_back();
        }
        return result;
    }

    // Absolute path (starts with /)
    if (url_without_fragment[0] == '/') {
        // Extract domain from hostname (without protocol)
        std::string domain = hostname;

        // Remove protocol if present
        if (domain.starts_with("http://")) {
            domain = domain.substr(7);
        } else if (domain.starts_with("https://")) {
            domain = domain.substr(8);
        }

        // Remove path from domain if present
        size_t path_pos = domain.find('/');
        if (path_pos != std::string::npos) {
            domain = domain.substr(0, path_pos);
        }

        std::string result = domain + url_without_fragment;
        // Remove trailing slash
        if (result.length() > 0 && result.back() == '/') {
            result.pop_back();
        }
        return result;
    }

    // Relative path - use url_join (which already handles trailing slash)
    return url_join(hostname, url_without_fragment);
}

std::string sanitize_url_for_filename(const std::string& url) {
    // Define problematic characters and their replacements
    const std::unordered_set<char> invalid_chars = {'/', '\\', ':', '*', '?', '"', '<',
                                                    '>', '|',  '#', '&', '=', '%'};

    std::string sanitized;
    sanitized.reserve(url.length());

    for (char c : url) {
        if (invalid_chars.find(c) != invalid_chars.end()) {
            // Replace invalid characters with underscore
            sanitized += '_';
        } else if (std::isspace(c)) {
            // Replace spaces with underscore
            sanitized += '_';
        } else {
            // Keep valid characters
            sanitized += c;
        }
    }

    // Handle edge cases
    if (sanitized.empty()) {
        sanitized = "index";
    }

    // Remove consecutive underscores
    sanitized.erase(std::unique(sanitized.begin(), sanitized.end(),
                                [](char a, char b) { return a == '_' && b == '_'; }),
                    sanitized.end());

    // Remove leading/trailing underscores
    if (!sanitized.empty() && sanitized.front() == '_') {
        sanitized.erase(0, 1);
    }
    if (!sanitized.empty() && sanitized.back() == '_') {
        sanitized.pop_back();
    }

    // Ensure filename isn't too long (leave room for .html extension)
    const size_t MAX_FILENAME_LENGTH = 250; // 255 - 5 for ".html"
    if (sanitized.length() > MAX_FILENAME_LENGTH) {
        // Keep the end of the URL as it's usually more specific
        sanitized = sanitized.substr(sanitized.length() - MAX_FILENAME_LENGTH);
    }

    return sanitized;
}

} // namespace krauler
