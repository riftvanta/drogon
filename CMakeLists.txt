cmake_minimum_required(VERSION 3.5)
project(drogon_webapp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimize build for Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Enable compiler optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
    add_definitions(-DDEBUG)
endif()

# Enable all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

find_package(Drogon CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} main.cc)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(views VIEW_SRC)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CTL_SRC}
    ${FILTER_SRC}
    ${MODEL_SRC}
    ${VIEW_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# Enable position independent code
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)