#include "MazeRenderer.h"

#include <iostream>

MazeRenderer::MazeRenderer() : clear_color(0.45f, 0.55f, 0.60f, 1.00f)
{
  initGLFW();
  hintGLFWWindow();
  createGLFWWindow();
  initImGui();
}

MazeRenderer::~MazeRenderer()
{
  cleanupGLFW();
  cleanupImGui();
}

void MazeRenderer::initGLFW()
{
  glfwSetErrorCallback([](int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  });

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void MazeRenderer::hintGLFWWindow()
{
  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // Required on Mac
#else
  // GL 3.0 + GLSL 130
  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

void MazeRenderer::createGLFWWindow()
{
  window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
  if (window == NULL)
    exit(EXIT_FAILURE);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);    // Enable vsync
}

void MazeRenderer::initImGui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void MazeRenderer::newFrame()
{
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void MazeRenderer::render()
{
  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void MazeRenderer::cleanupImGui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void MazeRenderer::cleanupGLFW()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}