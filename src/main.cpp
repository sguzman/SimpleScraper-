#include <iostream>
#include <fstream>
#include "redis-http.hxx"
#include "articles.hxx"
#include "items.pb.h"

constexpr static const unsigned short limit = 1276;
constexpr static const char * const itemFile = "./items.data";
static items::Links itemCache{};

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

inline static void run_me_at_exit() noexcept {
  {
    std::ofstream output{itemFile, std::ios::trunc | std::ios::binary};
    if (!itemCache.SerializeToOstream(&output)) {
      std::cerr << "Failed to write address book." << std::endl;
    } else {
      std::cout << "Wrote " << itemCache.links().size() << " entries to file" << std::endl;
    }
  }
}

int main() {
  {
    Redis::redis_init();
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    atexit(run_me_at_exit);
  }

  {
    std::ifstream input{itemFile, std::ios::binary};
    if (input.is_open()) {
      if (!itemCache.ParseFromIstream(&input)) {
        std::cerr << "Failed to parse address book." << std::endl;
        return EXIT_FAILURE;
      }
    }
  }

  {
    for (unsigned short i{1u}; i < limit; ++i) {
      const std::string path{"/page" + std::to_string(i)};
      std::array<std::string, 10> ls{};
      if (itemCache.links().find(path) == itemCache.links().cend()) {
        std::cout << "Generating links for page " << i << std::endl;
        ls = Article::articles(i);

        auto page = items::Page();
        {
          *page.mutable_link_1() = ls[0];
          *page.mutable_link_2() = ls[1];
          *page.mutable_link_3() = ls[2];
          *page.mutable_link_4() = ls[3];
          *page.mutable_link_5() = ls[4];
          *page.mutable_link_6() = ls[5];
          *page.mutable_link_7() = ls[6];
          *page.mutable_link_8() = ls[7];
          *page.mutable_link_9() = ls[8];
          *page.mutable_link_10() = ls[9];
        }
        itemCache.mutable_links()->operator[](path) = page;
      } else {
        const auto& item{itemCache.mutable_links()->operator[](path)};
        ls = std::array<std::string, 10> {{
          item.link_1(),
          item.link_2(),
          item.link_3(),
          item.link_4(),
          item.link_5(),
          item.link_6(),
          item.link_7(),
          item.link_8(),
          item.link_9(),
          item.link_10()
        }};
      }

      const auto& links{ls};
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