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
  void initGLFW();
  void hintGLFWWindow();
  void createGLFWWindow();
  void initImGui();
  void setNewFrame();
  void cleanupGLFW();
  void cleanupImGui();

  GLFWwindow *window;
  ImVec4 clear_color;
  const char *glsl_version;

  friend class MazeView;
};

#endif