// Sample app built with Dear ImGui and ImPlot
// This app uses implot and imgui, but does not output to any backend! It only
// serves as a proof that an app can be built, linked, and run.

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
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

#include <stdexcept>

#include <vector>

// Maze dimensions
const int MAZE_WIDTH = 50;
const int MAZE_HEIGHT = 30;

// Maze data (0 = path, 1 = wall)
std::vector<std::vector<int>> maze(MAZE_HEIGHT, std::vector<int>(MAZE_WIDTH, 0));

void renderMaze()
{
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImVec2 p = ImGui::GetCursorScreenPos();
  float cell_size = 20.0f;
  ImU32 grid_color = IM_COL32(100, 100, 100, 255);    // Dark grey for grid lines

  // Calculate total size of the maze
  float maze_width = MAZE_WIDTH * cell_size;
  float maze_height = MAZE_HEIGHT * cell_size;

  // Draw background
  draw_list->AddRectFilled(p, ImVec2(p.x + maze_width, p.y + maze_height), IM_COL32(255, 255, 255, 255));

  // Draw cells and vertical grid lines
  for (int x = 0; x <= MAZE_WIDTH; x++) {
    float x_pos = p.x + x * cell_size;
    draw_list->AddLine(ImVec2(x_pos, p.y), ImVec2(x_pos, p.y + maze_height), grid_color);

    if (x < MAZE_WIDTH) {
      for (int y = 0; y < MAZE_HEIGHT; y++) {
        if (maze[y][x] == 1) {
          ImVec2 cell_min = ImVec2(x_pos, p.y + y * cell_size);
          ImVec2 cell_max = ImVec2(cell_min.x + cell_size, cell_min.y + cell_size);
          draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(128, 64, 64, 255));
        }
      }
    }
  }

  // Draw horizontal grid lines
  for (int y = 0; y <= MAZE_HEIGHT; y++) {
    float y_pos = p.y + y * cell_size;
    draw_list->AddLine(ImVec2(p.x, y_pos), ImVec2(p.x + maze_width, y_pos), grid_color);
  }
}

void renderGUI()
{
  // Set initial window position and size, but allow movement
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_FirstUseEver);

  // Remove NoMove and NoResize flags
  ImGui::Begin("Maze Generator and Solver", nullptr, ImGuiWindowFlags_NoTitleBar);

  // Button layout
  ImGui::BeginGroup();
  if (ImGui::Button("Generate Maze (DFS)", ImVec2(200, 0))) {
    // Implement DFS maze generation
  }
  if (ImGui::Button("Generate Maze (Prim's)", ImVec2(200, 0))) {
    // Implement Prim's maze generation
  }
  if (ImGui::Button("Solve Maze (A*)", ImVec2(200, 0))) {
    // Implement A* maze solving
  }
  if (ImGui::Button("Solve Maze (BFS)", ImVec2(200, 0))) {
    // Implement BFS maze solving
  }
  ImGui::EndGroup();

  ImGui::SameLine();

  // Maze rendering
  ImGui::BeginChild("MazeView", ImVec2(0, 0), true);
  renderMaze();
  ImGui::EndChild();

  ImGui::End();
}

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char **)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
  if (window == NULL)
    return 1;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);    // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Our GUI code
    {
      ImGui::Begin("Maze Generator and Solver");

      // Render maze
      renderGUI();
      // renderMaze();

      ImGui::End();
    }

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

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}