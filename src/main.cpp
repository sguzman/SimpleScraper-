#include <iostream>
#include <cpp_redis/cpp_redis>
#include "http-get.hxx"

constexpr static const char * const host{"23.95.221.108"};

int main() {
  cpp_redis::client client;
  client.connect();

  client.set("hello", "42");
  {
    client.sync_commit();
  }

  auto helloFtr{client.get("hello")};
  {
    client.sync_commit();
  }
  std::cout << helloFtr.get() << std::endl;

  return EXIT_SUCCESS;
}