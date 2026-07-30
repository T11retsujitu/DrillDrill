cmake_minimum_required(VERSION 3.16)
project(DrillDrill LANGUAGES CXX)

# --- C++17 を必須にする ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# 学習用なので警告は多めに出す（-Werror は付けない: 学習の妨げになるため）
add_compile_options(-Wall -Wextra)

# ドリル（学習者が実装するセクション）をビルドするか。CI では ON のままビルド確認する。
option(DD_BUILD_DRILLS "Build the per-section drill targets" ON)

find_package(OpenCV REQUIRED)
message(STATUS "DrillDrill: OpenCV ${OpenCV_VERSION}")

include(cmake/DrillHelpers.cmake)

enable_testing()

add_subdirectory(common)
add_subdirectory(tools)
if(DD_BUILD_DRILLS)
  add_subdirectory(sections)
endif()
