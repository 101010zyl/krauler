#pragma once
#include <string>

namespace krauler {
// Performs HTTP GET and returns the response body. Throws std::runtime_error on failure.
std::string fetch_url(const std::string& url);
} // namespace krauler
