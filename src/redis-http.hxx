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
    if (map.find(path) == map.cend()) {
      const std::string html{HTTP::get(path.c_str())};
      client.hset("ebooks", path, html);
      {
        client.sync_commit();
        std::cout << path << " -> " << html.length() << std::endl;
      }

      return html;
    }

    return map[path];
  }

  inline static void redis_kill() noexcept {
    client.disconnect(true);
  }
}