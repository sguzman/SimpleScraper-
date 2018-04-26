cmake_minimum_required(VERSION 3.11)
project(scrape)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(scrapeDebug src/main.cpp src/curl.hxx)
link_directories(/usr/local/lib)
target_link_libraries(scrapeDebug curl)
target_link_libraries(scrapeDebug gumbo)