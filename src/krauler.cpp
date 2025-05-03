#include "krauler/krauler.hpp"

namespace krauler {

void Krauler::start() {
    try {
        std::string url  = config_.url;
        std::string html = krauler::fetch_url(url);
        std::cout << "Fetched " << html.size() << " bytes\n";

        auto links = krauler::extract_links(html);
        std::cout << "Extracted " << links.size() << " links:\n";
        for (const auto& link : links) {
            std::cout << link << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }
}
} // namespace krauler