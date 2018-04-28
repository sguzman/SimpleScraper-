#include <iostream>
#include <gumbo.h>
#include "article-links.hxx"

constexpr static const char * const indent_chars = " ";

int main() noexcept {
  const unsigned short page = 2;
  const auto pages = Article::links(page);
  for (const auto &href : pages) {
    std::cout << href << std::endl;
  }

  return EXIT_SUCCESS;
}