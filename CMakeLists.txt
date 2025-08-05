cmake_minimum_required(VERSION 3.30)
project(LearningOpenGL)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # 3.4
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(glfw)

add_library(glad
    ./glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ./glad/include
)

# Header files also need to be included for IDE
add_executable(${PROJECT_NAME} main.cpp
        Shader.cpp
        Shader.h
        stb_image.h
        stb_image.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)