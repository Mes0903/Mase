#ifndef MAZEIMGUIHANDLER_H
#define MAZEIMGUIHANDLER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>    // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
  !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class MazeRenderer {
public:
  MazeRenderer();
  ~MazeRenderer();
  void render();

private:
  void initGLFW__();
  void hintGLFWWindow__();
  void createGLFWWindow__();
  void initImGui__();
  void setNewFrame__();
  void cleanupGLFW__();
  void cleanupImGui__();

  GLFWwindow *window__;
  ImVec4 clear_color__;
  const char *glsl_version__;

  friend class MazeView;
};

#endif