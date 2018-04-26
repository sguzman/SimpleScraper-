#pragma once
#include <array>
#include <gumbo.h>

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