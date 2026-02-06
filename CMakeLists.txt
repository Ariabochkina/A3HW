cmake_minimum_required(VERSION 3.14)
project(hyperloglog_research CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(hll_experiment
  src/main.cpp
  src/random_stream_gen.cpp
  src/hash_func_gen.cpp
  src/hyperloglog.cpp
  src/hyperloglog_improved.cpp
)
target_include_directories(hll_experiment PRIVATE ${CMAKE_SOURCE_DIR}/src)
