
#include <VulkanPT/window.hpp>

Window::Window(const int in_width, const int in_height, std::string in_name) :
  width{ in_width }, height{ in_height }, name{ in_name }, window{ nullptr }
{
  Init();
}

Window::~Window()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::Init()
{
  //glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}
