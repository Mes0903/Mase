set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(
  MAZE_CORE STATIC
  ${MAZE_DIR}/src/MazeController.cpp
  ${MAZE_DIR}/src/MazeModel.cpp
  ${MAZE_DIR}/src/MazeView.cpp
  ${MAZE_DIR}/src/MazeRenderer.cpp
)

target_include_directories(
  MAZE_CORE
  PRIVATE
    ${THIRD_DIR}/imgui
    ${THIRD_DIR}/glfw/include
    ${THIRD_DIR}/glad/include
    ${MAZE_DIR}/include
    ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(
  MAZE_CORE
  IMGUI_LIB
  glfw
  glad
  ${OPENGL_LIBRARIES}
)