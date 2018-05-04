#pragma once
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream>

namespace HTTP {
  static const std::string getStr{"GET "};
  static const std::string endl{"\r\n"};
  constexpr static const unsigned short BUFFER_SIZE{1024};
  constexpr static const in_port_t port{80};
  constexpr static const char * const hostStr{"23.95.221.108"};

  inline static int socket_connect(const char * const host, in_port_t port) noexcept {
    constexpr const int on{1};

    struct hostent* hp;
    if((hp = gethostbyname(host)) == nullptr){
      herror("gethostbyname");
      exit(1);
    }

    struct sockaddr_in addr{};
    bcopy(hp->h_addr, &addr.sin_addr, static_cast<size_t>(hp->h_length));
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    const int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(int));

    if(sock == -1){
      perror("setsockopt");
      exit(1);
    }

    if(connect(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_in)) == -1){
      perror("connect");
      exit(1);
    }

    return sock;
  }

  inline static const std::string get(const char * const host, const char * const path) noexcept {
    const std::string url{getStr + path +  endl};

    int fd;
    char buffer[BUFFER_SIZE];

    fd = socket_connect(host, port);
    write(fd, url.c_str(), url.size());
    bzero(buffer, BUFFER_SIZE);

    std::string buf{};
    for (ssize_t size{read(fd, buffer, BUFFER_SIZE - 1)}; size != 0u; size = read(fd, buffer, BUFFER_SIZE - 1)) {
      buf += buffer;
      bzero(buffer, static_cast<size_t>(size));
    }

    shutdown(fd, SHUT_RDWR);
    close(fd);

    return buf;
  }

  inline static const std::string get(const char * const path) noexcept {
    return get(hostStr, path);
  }
}