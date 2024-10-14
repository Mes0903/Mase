cmake_minimum_required(VERSION 3.11)

project(Mase)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  message('OPENGL_FOUND-is-true')
else()
  message('OPENGL_FOUND-is-false')
endif()

if(WIN32)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    MESSAGE("==================== USING MSVC TO COMILE ====================")
    add_compile_options(/wd4819 /wd4244 /wd4267 /wd4305 "/Zc:__cplusplus")
    set(CMAKE_CXX_FLAGS_DEBUG "/O2")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
  else()
    MESSAGE("==================== USING MINGW TO COMILE ====================")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  endif()
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

set(THIRD_DIR ${PROJECT_SOURCE_DIR}/3rdparty)
set(MAZE_DIR ${PROJECT_SOURCE_DIR}/Maze)

add_subdirectory(${THIRD_DIR})
add_subdirectory(${MAZE_DIR})

add_executable(
  ${PROJECT_NAME} 
  ${PROJECT_SOURCE_DIR}/src/mase.cpp
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${THIRD_DIR}/imgui
    ${THIRD_DIR}/glfw/include
    ${THIRD_DIR}/glad/include
    ${MAZE_DIR}/include
    ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_NAME}
  IMGUI_LIB
  MAZE_CORE
  glfw
  glad
  ${OPENGL_LIBRARIES}
)
