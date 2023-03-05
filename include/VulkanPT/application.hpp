
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <VulkanPT/config.hpp>
#include <VulkanPT/window.hpp>
#include <VulkanPT/frame.hpp>

class Application
{
 public:
  Application();
  ~Application();

  static constexpr int width = 800;
  static constexpr int height = 600;

  void Init();
  void Run();

 private:

  void CreateInstance();
  void CreateDevice();

  Window window { width, height, "Hello Vulkan!!"};

  vk::Instance instance { nullptr };
  vk::DebugUtilsMessengerEXT debug_messenger { nullptr };
  vk::DispatchLoaderDynamic dispatch_loader;
  vk::SurfaceKHR surface;

  vk::PhysicalDevice physical_device { nullptr };
  vk::Device device { nullptr };
  vk::Queue graphics_queue { nullptr };
  vk::Queue present_queue { nullptr };

  vk::SwapchainKHR swapchain { nullptr };
  std::vector<VulkanUtils::SwapChainFrame> swapchain_frames;
  vk::Format swapchain_format;
  vk::Extent2D swapchain_extent;

};

#endif // APPLICATION_HPP
