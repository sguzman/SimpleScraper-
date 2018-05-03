#pragma once

#include <array>
#include <gumbo.h>
#include "redis-http.hxx"
#include "node.hxx"

namespace Article {
  constexpr static const std::array<const unsigned short, 6> ps{{2, 3, 1, 1, 1, 1}};
  constexpr static const std::array<const unsigned short, 10> cs{{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}};
  constexpr static const std::array<const unsigned short, 5> ccs{{1, 3, 1, 1, 1}};
  constexpr static const unsigned short n{10};
  const std::string path_prefix{"/page/"};

  inline static const std::array<std::string, n> articles(const unsigned short page) noexcept {
    const auto& html = Redis::redis_get(path_prefix + std::to_string(page));
    GumboOutput* output{gumbo_parse(html.c_str())};
    const auto& articles{Node::vector(output->root, ps)};
    const auto& artElem{Node::children(articles, cs)};
    const auto& links{Node::nodesAttr(artElem, ccs, "href")};
    std::array<std::string, n> out{};
    {
      for (auto i{0u}; i < links.size(); ++i) {
        out[i] = std::string{links[i]};
      }
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return out;
  }
}