#include <iostream>
#include "http-get.hxx"

constexpr static const char * const host{"23.95.221.108"};

int main() {
  const auto& html{HTTP::get(host)};
  std::cout << html.length() << std::endl;
  std::cout << html << std::endl;

  return EXIT_SUCCESS;
}