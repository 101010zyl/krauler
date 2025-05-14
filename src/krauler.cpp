#include "krauler/krauler.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

namespace krauler {

void Krauler::run() {
    try {
        std::string url = config_.url;

        std::string html = krauler::fetch_url(url);
        spdlog::info("Fetched {} bytes from {}", html.size(), url);

        Robotstxt robotstxt(url);
        Filter    filter(config_, robotstxt, visited_urls_);

        url_queue_.push(url);
        // while ((!url_queue_.empty()) && visited_urls_.size() < 10) {
        //     auto url = url_queue_.front();
        //     url_queue_.pop();
        //     auto html = krauler::fetch_url(url);
        //     visited_urls_.insert(url);
        //     save_html(url, html);
        //     saved_urls_.insert(url);

        //     auto links = krauler::extract_links(html);
        //     for (const auto& link : links) {
        //         // spdlog::debug("New link: {}", link);
        //         if (filter.is_allowed(link)) {
        //             url_queue_.push(link);
        //             spdlog::debug("Added \"{}\" to queue", link);
        //         } else {
        //             spdlog::debug("Blocked by robots.txt: \"{}\"", link);
        //             visited_urls_.insert(link);
        //         }
        //     }
        // }

    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
    }
}
void Krauler::save_html(const std::string& url, const std::string& html) {
    // Save the HTML content to a file or database
    std::string   filename = config_.output_dir + "/" + url + ".html";
    std::ofstream file(filename);
    if (file.is_open()) {
        file << html;
        file.close();
        spdlog::info("Saved HTML to {}", filename);
    } else {
        spdlog::error("Failed to save HTML to {}", filename);
    }
}
} // namespace krauler