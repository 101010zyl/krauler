// krauler/src/main.cpp

#include <curl/curl.h>
#include <cxxopts/cxxopts.hpp>
#include <gumbo.h>
#include <iostream>
#include <krauler/config.hpp>
#include <set>
#include <spdlog/spdlog.h>
#include <string>

// Write callback for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t       totalBytes = size * nmemb;
    std::string* output     = static_cast<std::string*>(userp);
    output->append(static_cast<char*>(contents), totalBytes);
    return totalBytes;
}

// Recursively search the DOM for <a> tags and extract href attributes
void extract_links(GumboNode* node, std::set<std::string>& links) {
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A &&
        (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        links.insert(href->value);
    }

    const GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        extract_links(static_cast<GumboNode*>(children->data[i]), links);
    }
}

int main(int argc, char* argv[]) {
    krauler::Config config = krauler::parse_args(argc, argv);

    CURL*       curl;
    CURLcode    res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Fetched " << response.size() << " bytes\n";

            // Parse HTML with gumbo
            GumboOutput*          output = gumbo_parse(response.c_str());
            std::set<std::string> links;
            extract_links(output->root, links);
            gumbo_destroy_output(&kGumboDefaultOptions, output);

            // Print extracted links
            std::cout << "Extracted " << links.size() << " links:\n";
            for (const auto& link : links) {
                std::cout << link << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
