#pragma once
#include <cxxopts/cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

namespace krauler {
struct Config {
    std::string url       = nullptr; // Starting URL
    int         max_depth = 1;       // Max crawl depth
    bool        verbose   = true;    // Enable verbose output
    bool        help      = false;   // Print help
    bool        ethical   = true;    // Ethical crawling

    Config() = default;
    Config(const cxxopts::ParseResult& result)
        : url(result["url"].as<std::string>()), max_depth(result["depth"].as<int>()),
          verbose(result["verbose"].as<bool>()), help(result["help"].as<bool>()),
          ethical(result["ethical"].as<bool>()) {
        // Print the parsed configuration
        spdlog::info("Parsed configuration: ");
        spdlog::info("  URL: {}", url);
        spdlog::info("  Max Depth: {}", max_depth);
        spdlog::info("  Verbose: {}", verbose);
        spdlog::info("  Help: {}", help);
        spdlog::info("  Ethical: {}", ethical);
        if (help) {
            spdlog::info("  Help requested");
        }
        if (url.empty()) {
            spdlog::error("URL is required");
            throw std::runtime_error("URL is required");
        }
        if (max_depth < 1) {
            spdlog::error("Max depth must be at least 1");
            throw std::runtime_error("Max depth must be at least 1");
        }
        if (verbose) {
            spdlog::set_level(spdlog::level::debug); // Set global log level to debug
        } else {
            spdlog::set_level(spdlog::level::info); // Set global log level to info
        }
        if (ethical) {
            spdlog::info("Ethical crawling enabled");
        } else {
            spdlog::warn("Ethical crawling disabled");
        }
    }
};

Config parse_args(int argc, char* argv[]);

} // namespace krauler
