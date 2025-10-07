cmake_minimum_required(VERSION 3.10.0)
project(radarscraper) 
add_subdirectory(cpr)
set(mysql-concpp_DIR "/usr/lib64/cmake/mysql-concpp")
find_package(mysql-concpp REQUIRED)
include_directories("include/")
include_directories(${CPR_INCLUDE_DIRS})
include_directories("/usr/include/mysql-cppconn")
set(CMAKE_CXX_STANDARD 23)
set(SOURCES
    src/main.cpp
    src/scraper.cpp
    src/parser.cpp
    src/database.cpp
    src/summary.cpp
    src/config.cpp 
    src/simdjson.cpp
)
add_compile_options(-O3)
add_executable(radarscraper ${SOURCES})
target_link_libraries(radarscraper mysql::concpp ${CPR_LIBRARIES})

