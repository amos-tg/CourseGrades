cmake_minimum_required(VERSION 3.16)

# Project name and language
project(MyProject LANGUAGES CXX)

# Set C++ standard (change to 17 or 20 if you prefer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add all source files
file(GLOB SOURCES "src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Add include directory
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

