#pragma once
#include <array>
#include <gumbo.h>

namespace Node {
  static inline const char * attr(__restrict const GumboNode* const root, __restrict const char * const att) noexcept {
    return gumbo_get_attribute(&root->v.element.attributes, att)->value;
  }

  template <unsigned long n>
  static inline const char * nodeAttr(__restrict const GumboNode * const root, const std::array<const unsigned short, n>& arr, __restrict const char * const att, const unsigned short idx = 0u) noexcept  {
    if (idx == n) {
      return attr(root, att);
    }
    return nodeAttr(static_cast<const GumboNode *>(root->v.element.children.data[arr[idx]]), arr, att, idx + 1);
  }

  template <unsigned long n, unsigned long m>
  static inline std::array<char *, n> nodesAttr(const std::array<const GumboInternalNode*, n>& roots, const std::array<const unsigned short, m>& arr, __restrict const char * const att) noexcept  {
    std::array<char* , n> nodes{};
    {
      for (auto i{0u}; i < n; ++i) {
        nodes[i] = const_cast<char*>(nodeAttr(roots[i], arr, att));
      }
    }
    return nodes;
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