cmake_minimum_required(VERSION 3.10)

project(engine VERSION 0.01 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

FILE(GLOB_RECURSE engine_kod src/*.cpp)
add_executable(engine
    ${engine_kod}

    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/backends/imgui_impl_sdl.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
    )
include_directories(
    include
    third_party/SDL2/include/SDL2
    third_party/SDL2_image/include/SDL2
    third_party/assimp/include/
    third_party/glm/0.9.9.8/include
    third_party/imgui
    third_party/imgui/backends
    third_party/BulletPhysics
    )
target_link_directories(engine PUBLIC third_party/SDL2/lib 
    third_party/SDL2_image/lib 
    third_party/assimp/lib 
    third_party/BulletPhysics/BulletCollision 
    third_party/BulletPhysics/BulletDynamics 
    third_party/BulletPhysics/LinearMath)

target_link_libraries(engine ${OPENGL_LIBRARIES})
target_link_libraries(engine SDL2)
target_link_libraries(engine SDL2_image)
target_link_libraries(engine assimp)
target_link_libraries(engine BulletDynamics)
target_link_libraries(engine BulletCollision)
target_link_libraries(engine LinearMath)

