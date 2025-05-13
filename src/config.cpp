#include "krauler/config.hpp"

#include <filesystem>
#include <iostream>

namespace krauler {

Config parse_args(int argc, char* argv[]) {
    try {
        cxxopts::Options options(argv[0], "A simple modular web crawler");
        options.add_options()("u,url", "Starting URL", cxxopts::value<std::string>())(
            "d,depth", "Max crawl depth", cxxopts::value<int>()->default_value("1"))(
            "t,threads", "Number of threads", cxxopts::value<int>()->default_value("4"))(
            "v,verbose", "Enable verbose output", cxxopts::value<bool>()->default_value("false"))(
            "e,ethical", "Enable ethical crawling", cxxopts::value<bool>()->default_value("true"))(
            "m,sitemap", "Follow sitemap", cxxopts::value<bool>()->default_value("false"))(
            "h,help", "Print help", cxxopts::value<bool>()->default_value("false"));

        auto result = options.parse(argc, argv);
        if (result.count("help") || !result.count("url")) {
            std::cout << options.help() << std::endl;
            std::exit(0);
        }

        return Config(result);
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        std::exit(1);
    }
}

} // namespace krauler
