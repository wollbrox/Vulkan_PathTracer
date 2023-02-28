
#include <VulkanPT/application.hpp>
#include <VulkanPT/instance.hpp>
#include <VulkanPT/logging.hpp>

Application::Application() {}

Application::~Application()
{
  instance.destroyDebugUtilsMessengerEXT(debug_messenger, nullptr, dispatch_loader);
  instance.destroy();
}

void Application::Init()
{
  MakeInstance();
  MakeDebugMessenger();
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
  instance = VulkanInit::MakeInstance("Vulkan Path Tracer");
  dispatch_loader = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
}

void Application::MakeDebugMessenger()
{
  debug_messenger = VulkanInit::MakeDebugMessenger(instance, dispatch_loader);
}
