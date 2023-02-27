
#include <VulkanPT/application.hpp>
#include <VulkanPT/instance.hpp>

Application::Application() {}

Application::~Application()
{
  instance.destroy();
}

void Application::Init()
{
  glfwInit();
  MakeInstance();
}

void Application::Run()
{
  /*while (!window.Close())
  {
    glfwPollEvents();

  }*/
}

void Application::MakeInstance()
{
  instance = VulkanInit::MakeInstance(true, "Vulkan Path Tracer");

}
