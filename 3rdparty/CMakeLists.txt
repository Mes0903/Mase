include(FetchContent)
set(IMGUI_DIR ${THIRD_DIR}/imgui)
set(IMFD_DIR ${THIRD_DIR}/ImGuiFileDialog)

# if(UNIX)
#   set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) # disable docs
#   set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE) # disable tests
#   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # disable examples
#   set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE) # disable Wayland support
# endif()

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(imgui)