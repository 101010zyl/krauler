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

} // namespace krauler
