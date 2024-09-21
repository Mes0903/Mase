include(FetchContent)
set(IMGUI_DIR ${THIRD_DIR}/imgui)
set(IMPLOT_DIR ${THIRD_DIR}/implot)
set(IMFD_DIR ${THIRD_DIR}/ImGuiFileDialog)

# if(UNIX)
#   set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) # 禁用文檔構建
#   set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE) # 禁用測試構建
#   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # 禁用示例構建
#   set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE) # 禁用 Wayland 支持
# endif()

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(implot)