cmake_minimum_required(VERSION 3.1)
project(BattleWithLandlord)
set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(battlewithlandlord
    src/main.cc
    src/battlewithlandlord.cc
)

target_link_libraries(battlewithlandlord
    pthread
)
