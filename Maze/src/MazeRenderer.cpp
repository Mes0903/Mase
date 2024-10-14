#include "MazeRenderer.h"

#include <iostream>

MazeRenderer::MazeRenderer() : clear_color__(0.45f, 0.55f, 0.60f, 1.00f)
{
  initGLFW__();
  hintGLFWWindow__();
  createGLFWWindow__();
  initImGui__();
}

MazeRenderer::~MazeRenderer()
{
  cleanupImGui__();
  cleanupGLFW__();
}

void MazeRenderer::initGLFW__()
{
  glfwSetErrorCallback([](int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  });

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void MazeRenderer::hintGLFWWindow__()
{
  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  glsl_version__ = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  glsl_version__ = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // Required on Mac
#else
  // GL 3.0 + GLSL 130
  glsl_version__ = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

void MazeRenderer::createGLFWWindow__()
{
  window__ = glfwCreateWindow(1600, 900, "Mase", NULL, NULL);
  if (window__ == NULL)
    exit(EXIT_FAILURE);

  glfwMakeContextCurrent(window__);
  glfwSwapInterval(0);    // Enable vsync
}

void MazeRenderer::initImGui__()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window__, true);
  ImGui_ImplOpenGL3_Init(glsl_version__);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void MazeRenderer::setNewFrame__()
{
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void MazeRenderer::render__()
{
  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window__, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color__.x * clear_color__.w, clear_color__.y * clear_color__.w, clear_color__.z * clear_color__.w, clear_color__.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window__);
}

void MazeRenderer::cleanupImGui__()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void MazeRenderer::cleanupGLFW__()
{
  glfwDestroyWindow(window__);
  glfwTerminate();
}