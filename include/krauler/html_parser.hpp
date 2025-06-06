#pragma once
#include <set>
#include <string>

namespace krauler {
// Extracts all <a href="..."> links from the given HTML content
std::set<std::string> extract_links(const std::string& html);
std::string           url_join(const std::string& base, const std::string& path);
std::string           normalize_url(const std::string& hostname, const std::string& relative_url);
std::string           sanitize_url_for_filename(const std::string& url);
} // namespace krauler
