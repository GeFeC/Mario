cmake_minimum_required(VERSION 3.10)

project(mario)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE sources 
	src/*.cpp 
	src/glad.c
)

add_executable(${PROJECT_NAME} ${sources})

set(include_dirs
  src
  vendor/glfw/include 
  vendor/freetype/include
  vendor/include
)

set(lib_dirs lib)

set(libs 
  glfw
  pthread
  freetype
)

target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})
target_link_directories(${PROJECT_NAME} PUBLIC ${lib_dirs})
target_link_libraries(${PROJECT_NAME} PUBLIC ${libs})

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/freetype)

target_compile_options(${PROJECT_NAME} PUBLIC ${FLAGS})

add_dependencies(glfw freetype)

