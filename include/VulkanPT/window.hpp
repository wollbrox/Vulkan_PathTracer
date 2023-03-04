
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <GLFW/glfw3.h>

class Window
{
 public:
  Window(const int in_width, const int in_height, std::string in_name);
  ~Window();

  bool Close() { return glfwWindowShouldClose(window); }
  GLFWwindow* getInstance();

 private:
  
  void Init();

  const int width;
  const int height;

  std::string name;
  GLFWwindow *window;
};

#endif // WINDOW_HPP
