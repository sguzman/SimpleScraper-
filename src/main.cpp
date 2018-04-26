#include <iostream>
#include <array>
#include <gumbo.h>
#include "curl.hxx"
#include "pretty-print.hxx"
#include "node.hxx"
#include "article-links.hxx"

constexpr static const char * const url = "http://23.95.221.108/";
constexpr static const char * const indent_chars = " ";

int main() noexcept {
  const auto&& s = Curl::get(url);
  const GumboOutput * const output = gumbo_parse(s.c_str());
  const auto& articles{Node::vector(output->root, Article::ps)};
  const auto& artElem{Node::children(articles, Article::cs)};

  for (const auto &ele : artElem) {
    std::cout << Node::nodeAttr(ele, Article::ccs, "href") << std::endl;
  }

  gumbo_destroy_output(&kGumboDefaultOptions, const_cast<GumboOutput *>(output));
  return EXIT_SUCCESS;
}