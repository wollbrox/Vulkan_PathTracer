
#include <VulkanPT/application.hpp>
#include <VulkanPT/instance.hpp>
#include <VulkanPT/logging.hpp>
#include <VulkanPT/device.hpp>

Application::Application() {}

Application::~Application()
{
  device.destroySwapchainKHR(swapchain);
  device.destroy();
  instance.destroySurfaceKHR(surface);

#ifdef DEBUG
  instance.destroyDebugUtilsMessengerEXT(debug_messenger, nullptr, dispatch_loader);
#endif // DEBUG

  instance.destroy();
}

void Application::Init()
{
  CreateInstance();
  CreateDevice();
}

void Application::Run()
{
  /*while (!window.Close())
  {
    glfwPollEvents();

  }*/
}

void Application::CreateInstance()
{
  instance = VulkanInit::MakeInstance("Vulkan Path Tracer");
  dispatch_loader = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

#ifdef DEBUG
  debug_messenger = VulkanInit::MakeDebugMessenger(instance, dispatch_loader);
#endif // DEBUG

  VkSurfaceKHR c_style_surface;
  if (glfwCreateWindowSurface(instance, window.getInstance(), nullptr, &c_style_surface) != VK_SUCCESS)
    Debug::Error("Failed to abstract the glfw surface for Vulkan!");
  else
    Debug::Log("Successfully abstracted the glfw surface for Vulkan!");

  surface = c_style_surface;
}

void Application::CreateDevice()
{
  physical_device = VulkanInit::ChoosePhysicalDevice(instance);
  device = VulkanInit::CreateLogicalDevice(physical_device, surface);
  std::array<vk::Queue, 2> queues = VulkanInit::getQueues(physical_device, surface, device);
  graphics_queue = queues[0];
  present_queue = queues[1];

  VulkanInit::SwapChainBundle bundle = VulkanInit::CreateSwapchain(device,
                                                                   physical_device,
                                                                   surface,
                                                                   width, height);
  swapchain = bundle.swapchain;
  swapchain_images = bundle.images;
  swapchain_format = bundle.format;
  swapchain_extent = bundle.extent;
}
