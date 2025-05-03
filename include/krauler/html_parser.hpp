#pragma once
#include <set>
#include <string>

namespace krauler {
// Extracts all <a href="..."> links from the given HTML content
std::set<std::string> extract_links(const std::string& html);
} // namespace krauler
