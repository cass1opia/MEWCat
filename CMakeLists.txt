cmake_minimum_required(VERSION 3.15...3.30)
project(mewcat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(mewcat
  python/bindings.cpp
  mewcat.cpp
  weighted_graph.cpp
  phased_local_search.cpp
  phased_local_search_adjmatrix.cpp
  phased_local_search_adjlist.cpp
)

target_include_directories(mewcat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS mewcat DESTINATION .)
