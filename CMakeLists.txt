cmake_minimum_required(VERSION 3.10)
project(CuckooFilter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(cuckoo_filter
    main.cpp
    src/cuckoo_filter.cpp
)
