#pragma once
#include <iostream>
#include <cpp_redis/cpp_redis>
#include "http-get.hxx"

namespace Redis {
  static cpp_redis::client client;
  static std::unordered_map<std::string, std::string> map{};
  static const std::string host{"23.95.221.108"};

  inline static void redis_init() noexcept {
    client.connect();

    {
      auto ftr{client.hgetall("ebooks")};
      {
        client.sync_commit();
      }
      const auto& get{ftr.get()};
      const auto& arr{get.as_array()};
      for (auto i{0u}; i < arr.size(); i += 2) {
        const auto& key{arr[i].as_string()};
        const auto& val{arr[i + 1].as_string()};
        map[key] = val;
      }
    }

    std::cout << "Found " << map.size() << " Http entries in cache" << std::endl;
  }

  inline static const std::string redis_get(const std::string& path) noexcept {
    const std::string url{host + path};

    const bool isMissing{map.find(url) == map.cend()};
    const std::string& html{isMissing? HTTP::get(host.c_str(), path.c_str()): map[url]};

    if (isMissing) {
      client.hset("ebooks", url, html);
      {
        client.sync_commit();
        std::cout << "Inserted key -> " << host << " with len(value) -> " << html.length() << std::endl;
      }
    }

    return html;
  }

  inline static void redis_kill() noexcept {
    client.disconnect(true);
  }
}