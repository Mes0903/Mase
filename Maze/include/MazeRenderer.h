#ifndef MAZEIMGUIHANDLER_H
#define MAZEIMGUIHANDLER_H

/**
 * @file MazeRenderer.h
 * @author Mes (mes900903@gmail.com)
 * @brief The main renderer of the maze, using to host the ImGui and OpenGL environment.
 * @version 0.1
 * @date 2024-10-14
 */

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
  friend class MazeView;

private:
  MazeRenderer();
  ~MazeRenderer();
  void render__();

private:
  void initGLFW__();
  void hintGLFWWindow__();
  void createGLFWWindow__();
  void initImGui__();
  void setNewFrame__();
  void cleanupGLFW__();
  void cleanupImGui__();

private:
  GLFWwindow *window__;
  ImVec4 clear_color__;
  const char *glsl_version__;
};

#endif