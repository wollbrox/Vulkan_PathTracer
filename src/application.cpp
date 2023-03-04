
#include <VulkanPT/application.hpp>
#include <VulkanPT/instance.hpp>
#include <VulkanPT/logging.hpp>
#include <VulkanPT/device.hpp>

Application::Application() {}

Application::~Application()
{
  device.destroy();

#ifdef DEBUG
  instance.destroyDebugUtilsMessengerEXT(debug_messenger, nullptr, dispatch_loader);
#endif // DEBUG

  instance.destroy();
}

void Application::Init()
{
  MakeInstance();
  MakeDevice();
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

#ifdef DEBUG
  debug_messenger = VulkanInit::MakeDebugMessenger(instance, dispatch_loader);
#endif // DEBUG
}

void Application::MakeDevice()
{
  physical_device = VulkanInit::ChoosePhysicalDevice(instance);
  device = VulkanInit::CreateLogicalDevice(physical_device);
  graphics_queue = VulkanInit::getQueue(physical_device, device);
}
