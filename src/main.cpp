#include <iostream>
#include <fstream>
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
  {
    Redis::redis_init();
  }

  {
    for (unsigned short i{1u}; i < limit; ++i) {
      const std::string path{"/page" + std::to_string(i)};
      const auto& links{Article::articles(i)};
      for (const auto &j : links) {
        const auto& host_path{lstrip(j, "https://it-eb.com")};
        Redis::redis_get(host_path.c_str());
      }
    }
  }

  {
    Redis::redis_kill();
    return EXIT_SUCCESS;
  }
}