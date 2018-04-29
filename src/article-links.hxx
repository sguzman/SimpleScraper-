#pragma once
#include <array>
#include <gumbo.h>
#include "node.hxx"
#include "curl.hxx"

namespace Article {
  constexpr static const char * const urlBase = "http://23.95.221.108/page/";
  constexpr static const unsigned short limit = 1276;
  constexpr static const std::array<const unsigned short, 6> ps{{2, 3, 1, 1, 1, 1}};
  constexpr static const std::array<const unsigned short, 10> cs{{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}};
  constexpr static const std::array<const unsigned short, 5> ccs{{1, 3, 1, 1, 1}};

  static inline const std::array<const std::string, 10> links(const unsigned short page) noexcept {
    const auto url{urlBase + std::to_string(page) + '/'};
    const auto&& s{Curl::get(url)};
    const GumboOutput * const output = gumbo_parse(s.c_str());
    const auto& articles{Node::vector(output->root, Article::ps)};
    const auto& artElem{Node::children(articles, Article::cs)};

    const std::array<const std::string, 10> links{{
      std::string{Node::nodeAttr(artElem[0], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[1], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[2], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[3], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[4], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[5], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[6], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[7], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[8], Article::ccs, "href")},
      std::string{Node::nodeAttr(artElem[9], Article::ccs, "href")}
    }};

    gumbo_destroy_output(&kGumboDefaultOptions, const_cast<GumboOutput *>(output));

    return links;
  }
}