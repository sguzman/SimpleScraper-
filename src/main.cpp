#include <iostream>
#include <array>
#include <gumbo.h>
#include "curl.hxx"
#include "pretty-print.hxx"

constexpr static const char * const url = "http://23.95.221.108/";
constexpr static const char * const indent_chars = " ";

namespace Article {
  constexpr static const std::array<const unsigned short, 6> ps{{2, 3, 1, 1, 1, 1}};
  constexpr static const std::array<const unsigned short, 10> cs{{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}};
  constexpr static const std::array<const unsigned short, 5> ccs{{1, 3, 1, 1, 1}};
}

namespace Node {
  static inline const char * attr(__restrict const GumboNode* const root, __restrict const char * const att) noexcept {
    return gumbo_get_attribute(&root->v.element.attributes, att)->value;
  }

  template <unsigned long n>
  static inline const GumboNode * node(__restrict const GumboNode * const root, const std::array<const unsigned short, n>& arr, const unsigned short idx = 0u) noexcept  {
    if (idx == n) {
      return root;
    }
    return node(static_cast<const GumboNode *>(root->v.element.children.data[arr[idx]]), arr, idx + 1);
  }

  template <unsigned long n>
  static inline const GumboVector * vector(__restrict const GumboNode * const root, const std::array<const unsigned short, n>& arr, const unsigned short idx = 0u) noexcept  {
    if (idx == n) {
      return &(root->v.element.children);
    }
    return vector(static_cast<const GumboNode *>(root->v.element.children.data[arr[idx]]), arr, idx + 1);
  }

  template <unsigned long n>
  static inline const std::array<const GumboNode *, n> children(__restrict const GumboVector * const root, const std::array<const unsigned short, n>& arr) noexcept {
    std::array<const GumboNode *, n> nodes{};
    {
      for (auto i = 0u; i < n; ++i) {
        nodes[i] = static_cast<const GumboNode*>(root->data[arr[i]]);
      }
    }
    return nodes;
  }
}

int main() noexcept {
  const auto&& s = Curl::get(url);
  const GumboOutput * const output = gumbo_parse(s.c_str());
  const auto& articles{Node::vector(output->root, Article::ps)};
  const auto& artElem{Node::children(articles, Article::cs)};
  const auto& first{Node::node(artElem[0], Article::ccs)};
  const auto href{Node::attr(first, "href")};
  std::cout << href << std::endl;

  for (const auto &ele : artElem) {
    std::cout << prettyprint(ele, 1, indent_chars) << std::endl;
  }

  gumbo_destroy_output(&kGumboDefaultOptions, const_cast<GumboOutput *>(output));
  return EXIT_SUCCESS;
}