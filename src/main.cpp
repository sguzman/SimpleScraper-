#include <iostream>
#include "redis-http.hxx"
#include "articles.hxx"

constexpr static const unsigned short limit = 1276;

inline static const std::string lstrip(const std::string& str, const std::string& prefix) noexcept {
  unsigned short idx{};
  {
    for (; idx < prefix.length(); ++idx) {
      if (str[idx] != prefix[idx]) {
        return str.substr(idx, std::string::npos);
      }
    }
  }

  return str.substr(idx, std::string::npos);
}

int main() {
  std::__1::ostream::sync_with_stdio(false);
  Redis::redis_init();

  for (unsigned short i{1u}; i < limit; ++i) {
    const auto& links{Article::articles(i)};
    for (const auto &j : links) {
      const auto& path{lstrip(j, "https://it-eb.com")};
      Redis::redis_get(path.c_str());
    }
  }

  Redis::redis_kill();
  return EXIT_SUCCESS;
}