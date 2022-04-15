cmake_minimum_required(VERSION 3.0)
project(Engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#------ RAYLIB Options ------
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
set(SUPPORT_CAMERA_SYSTEM OFF CACHE BOOL "" FORCE)
set(SUPPORT_FILEFORMAT_XM OFF CACHE BOOL "" FORCE)
set(SUPPORT_MESH_GENERATION OFF CACHE BOOL "" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "" FORCE)
#----------------------------

add_subdirectory(libs/raylib)
add_subdirectory(libs/pugixml)
add_subdirectory(engine)

add_executable(Engine ${ENGINE_SRC})
target_link_libraries(Engine PRIVATE raylib pugixml)
target_compile_definitions(Engine PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
