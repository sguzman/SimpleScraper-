#include <iostream>
#include <array>
#include <gumbo.h>
#include "redis-http.hxx"
#include "pretty-print.hxx"
#include "node.hxx"

constexpr static const unsigned short limit = 1276;
constexpr static const std::array<const unsigned short, 6> ps{{2, 3, 1, 1, 1, 1}};
constexpr static const std::array<const unsigned short, 10> cs{{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}};
constexpr static const std::array<const unsigned short, 5> ccs{{1, 3, 1, 1, 1}};

int main() {
  Redis::redis_init();

  const std::string path_prefix{"/page/"};
  for (unsigned short i{1u}; i < limit; ++i) {
    const auto& html = Redis::redis_get(path_prefix + std::to_string(i));
    GumboOutput* output{gumbo_parse(html.c_str())};
    const auto& articles{Node::vector(output->root, ps)};
    const auto& artElem{Node::children(articles, cs)};
    const auto& links{Node::nodesAttr(artElem, ccs, "href")};
    for (const auto &j : links) {
      std::cout << j << std::endl;
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
  }


  Redis::redis_kill();
  return EXIT_SUCCESS;
}