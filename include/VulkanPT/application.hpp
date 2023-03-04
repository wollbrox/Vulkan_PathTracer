
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <VulkanPT/window.hpp>
#include <VulkanPT/config.hpp>

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

  void MakeInstance();
  void MakeDevice();

  Window window { width, height, "Hello Vulkan!!"};

  vk::Instance instance { nullptr };
  vk::DebugUtilsMessengerEXT debug_messenger { nullptr };
  vk::DispatchLoaderDynamic dispatch_loader;

  vk::PhysicalDevice physical_device { nullptr };
  vk::Device device { nullptr };
  vk::Queue graphics_queue { nullptr };

};

#endif // APPLICATION_HPP
