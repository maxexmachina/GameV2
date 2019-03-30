cmake_minimum_required(VERSION 3.12)
project(Suffering2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")

add_executable(Suffering2 main.cpp libs.h shader.h camera.h)

target_link_libraries(Suffering2 "/usr/local/lib/libglfw.3.3.dylib")
include_directories("/usr/local/Cellar/glfw/3.2.1/include/GLFW")

include_directories("/Users/savage/Documents/stb_image/")

target_link_libraries(Suffering2 "/Users/savage/Documents/glad/libglad.dylib")
include_directories("/Users/savage/Documents/glad/include/")

include_directories("/usr/local/Cellar/glm/0.9.9.3/include/")