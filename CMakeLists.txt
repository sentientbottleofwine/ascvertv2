cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )

include(FetchContent)
FetchContent_Declare(argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)

FetchContent_MakeAvailable(argparse)

include_directories(${OpenCV_INCLUDE_DIRS})

project(ascvert)

add_executable(ascvert
    src/main.cpp
    src/ascvert.cpp 
)

target_link_libraries(ascvert
  PRIVATE argparse
  ${OpenCV_LIBS}
)
