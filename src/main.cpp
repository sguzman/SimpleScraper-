#include <iostream>
#include "redis-http.hxx"

constexpr static const char * const path{"/page/2"};

int main() {
  Redis::redis_init();

  const auto& html = Redis::redis_get(path);
  std::cout << html << std::endl;


  Redis::redis_kill();
  return EXIT_SUCCESS;
}