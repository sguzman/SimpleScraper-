#include <iostream>
#include <gumbo.h>
#include "redis-http.hxx"
#include "articles.hxx"

constexpr static const unsigned short limit = 1276;

int main() {
  Redis::redis_init();

  for (unsigned short i{1u}; i < limit; ++i) {
    const auto& links{Article::articles(i)};
    for (const auto &j : links) {
      std::cout << j << std::endl;
    }
  }


  Redis::redis_kill();
  return EXIT_SUCCESS;
}