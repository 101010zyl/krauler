#include "krauler/html_parser.hpp"

#include <gumbo.h>

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
    if (path.empty() || path == "/") {
        return base;
    }
    std::string result = base;

    // 处理 base: 确保不以多个 / 结尾
    while (!result.empty() && result.back() == '/')
        result.pop_back();

    // 处理 path: 去除前导的多个 /
    size_t path_start = 0;
    while (path_start < path.size() && path[path_start] == '/')
        ++path_start;

    // 加入 /
    result += "/";
    result += path.substr(path_start);

    return result;
}

std::string normalize_url(const std::string& hostname, const std::string& relative_url) {
    if (relative_url[0] == '/') {
        return url_join(hostname, relative_url);
    }
    if (relative_url.starts_with("http://") or relative_url.starts_with("https://")) {
        return relative_url;
    }
    // delete # and everything after it
    if (relative_url.find('#') != std::string::npos) {
        std::string new_relative_url = relative_url.substr(0, relative_url.find('#'));
        return url_join(hostname, new_relative_url);
    }
    return url_join(hostname, relative_url);
}

} // namespace krauler
