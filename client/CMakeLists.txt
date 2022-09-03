cmake_minimum_required(VERSION 3.21)
project(client)

set(CMAKE_CXX_STANDARD 17)

# Add Boost
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS system thread)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

add_executable(client main.cpp client.cpp registerer.cpp utils.cpp ../shared/request.cpp)
target_link_libraries(client ${USED_LIBS})
