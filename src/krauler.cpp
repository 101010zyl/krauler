#include "krauler/krauler.hpp"

#include <spdlog/spdlog.h>

namespace krauler {

void Krauler::start() {
    try {
        std::string url  = config_.url;
        std::string html = krauler::fetch_url(url);
        spdlog::info("Fetched {} bytes from {}", html.size(), url);

        auto links = krauler::extract_links(html);
        spdlog::info("Extracted {} links", links.size());
        std::cout << "Extracted " << links.size() << " links\n";
        for (const auto& link : links) {
            spdlog::info("Link: {}", link);
        }
    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
    }
}
} // namespace krauler