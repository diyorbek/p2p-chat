cmake_minimum_required(VERSION 3.21)
project(rendezvous)

set(CMAKE_CXX_STANDARD 17)

# Add Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)

find_package(Boost 1.42.0 REQUIRED COMPONENTS system thread)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

add_executable(rendezvous main.cpp server.cpp)
target_link_libraries(rendezvous ${USED_LIBS})
