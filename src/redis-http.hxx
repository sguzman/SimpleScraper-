#pragma once
#include <iostream>
#include <cpp_redis/cpp_redis>
#include <brotli/encode.h>
#include <brotli/decode.h>
#include "http-get.hxx"

namespace Redis {
  constexpr static const unsigned int buf_size{50000u};

  static cpp_redis::client client;
  static std::unordered_map<std::string, std::string> map{};

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
      size_t size{buf_size};
      uint8_t out_buffer[buf_size];
      {
        uint8_t in_buffer[buf_size];
        strcpy(reinterpret_cast<char *>(in_buffer), html.c_str());
        BrotliEncoderCompress(BROTLI_MAX_QUALITY,
                                               BROTLI_DEFAULT_WINDOW,
                                               BROTLI_MODE_TEXT,
                                               html.size(),
                                               in_buffer,
                                               &size,
                                               out_buffer
        );
      }

      std::string new_str{reinterpret_cast<const char * const>(out_buffer), size};
      client.hset("ebooks", path, new_str);
      {
        client.sync_commit();
        std::cout << path << " -> " << new_str.length() << std::endl;
      }

      return html;
    }

    {
      {
        std::cout << "Hit cache for " << path << std::endl;
      }

      const std::string str{map[path]};
      std::string out{};
      out.resize(buf_size);
      {
        size_t size{str.length()};
        uint8_t out_buffer[buf_size];
        {
          for (auto i{0u}; i < str.length(); ++i) {
            out_buffer[i] = static_cast<uint8_t>(str[i]);
          }
        }
        size_t out_size{buf_size};
        BrotliDecoderDecompress(size, out_buffer, &out_size, reinterpret_cast<uint8_t *>(out.data()));
      }

      return out;
    }
  }

  inline static void redis_kill() noexcept {
    client.disconnect(true);
  }
}