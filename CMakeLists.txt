cmake_minimum_required(VERSION 3.10.0)
project(radarscraper) 
set(mysql-concpp_DIR "/usr/lib64/cmake/mysql-concpp")
find_package(mysql-concpp)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG da40186618909b1a7363d4e4495aa899c6e0eb75)
FetchContent_MakeAvailable(cpr)
include_directories("include/")
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
add_compile_definitions("Analytics")
add_compile_options(-O2) # Doesn't have to be optimized much
add_executable(rscrap ${SOURCES})
target_link_libraries(rscrap mysql::concpp cpr::cpr )

