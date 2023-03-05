
#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <VulkanPT/config.hpp>

namespace VulkanInit
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool IsComplete()
    { return graphics_family.has_value() && present_family.has_value(); }
  };

  struct SwapChainSupportDetails
  {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> present_modes;
  };

  struct SwapChainBundle
  {
    vk::SwapchainKHR swapchain;
    std::vector<vk::Image> images;
    vk::Format format;
    vk::Extent2D extent;
  };

  void LogDeviceProperties(const vk::PhysicalDevice& device)
  {
    vk::PhysicalDeviceProperties properties = device.getProperties();
    
    Debug::Log("Device name: %s", properties.deviceName);
    Debug::Log("Device type:");
    switch (properties.deviceType)
    {
      case vk::PhysicalDeviceType::eCpu: Debug::Log("CPU"); break;
      case vk::PhysicalDeviceType::eDiscreteGpu: Debug::Log("Discrete GPU"); break;
      case vk::PhysicalDeviceType::eIntegratedGpu: Debug::Log("Integrated GPU"); break;
      case vk::PhysicalDeviceType::eVirtualGpu: Debug::Log("Virtual GPU"); break;
      default: Debug::Log("Other"); break;
    }
  }

  bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device,
                                   const std::vector<const char*>& requested_extensions)
  {
    std::set<std::string> required_extensions(requested_extensions.begin(), requested_extensions.end());
    Debug::Log("Device can support extensions:");

    for (vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties())
    {
      Debug::Log("%s", extension.extensionName);
      required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
  }

  bool IsSuitable(const vk::PhysicalDevice& device)
  {
    Debug::Log("Checking if device is suitable!");

    const std::vector<const char*> requested_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    Debug::Log("Requesting device extensions:");
    for (const char* extension : requested_extensions)
      Debug::Log("%s", extension);

    if (bool extension_supported = CheckDeviceExtensionSupport(device, requested_extensions))
      Debug::Log("Device can support the requested extension!");
    else
    {
      Debug::Log("Device can't support the requested extension!");
      return false;
    }
    
    return true;
  }

  vk::PhysicalDevice ChoosePhysicalDevice(vk::Instance& in_instance)
  {
    Debug::Log("Choosing physical device...");

    std::vector<vk::PhysicalDevice> available_devices = in_instance.enumeratePhysicalDevices();
    Debug::Log("There are %i physical devices available on this system!", available_devices.size());

    for (vk::PhysicalDevice device : available_devices)
    {
      LogDeviceProperties(device);
      if (IsSuitable(device)) return device;
    }

    return nullptr;
  }

  QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface)
  {
    QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queue_families = device.getQueueFamilyProperties();
    Debug::Log("System can support %i queue families!", queue_families.size());

    int index = 0;
    for (vk::QueueFamilyProperties queue_family : queue_families)
    {
      if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics)
      {
        indices.graphics_family = index;
        Debug::Log("Queue Family %i is suitable for graphics!", index);
      }

      if (device.getSurfaceSupportKHR(index, surface))
      {
        indices.present_family = index;
        Debug::Log("Queue Family %i is suitable for presenting!", index);
      }

      if (indices.IsComplete()) break;

      index++;
    }

    return indices;
  }

  vk::Device CreateLogicalDevice(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface)
  {
    QueueFamilyIndices indices = FindQueueFamilies(physical_device, surface);
    
    std::vector<uint32_t> unique_indices;
    unique_indices.push_back(indices.graphics_family.value());
    if (indices.graphics_family.value() != indices.present_family.value())
      unique_indices.push_back(indices.present_family.value());

    float queue_priority = 1.0f;
    std::vector<vk::DeviceQueueCreateInfo> queue_create_info;
    for (uint32_t queue_family_index : unique_indices)
      queue_create_info.emplace_back(vk::DeviceQueueCreateFlags(),
                                     queue_family_index,
                                     1, &queue_priority);

    std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    vk::PhysicalDeviceFeatures device_features = vk::PhysicalDeviceFeatures();
    
    std::vector<const char*> enabled_layers;
#ifdef DEBUG
    enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif // DEBUG
    
    vk::DeviceCreateInfo device_info = vk::DeviceCreateInfo(vk::DeviceCreateFlags(),
                                                            static_cast<int>(queue_create_info.size()),
                                                            queue_create_info.data(),
                                                            static_cast<int>(enabled_layers.size()),
                                                            enabled_layers.data(),
                                                            static_cast<int>(device_extensions.size()),
                                                            device_extensions.data(),
                                                            &device_features);
    try
    {
      vk::Device device = physical_device.createDevice(device_info);
      Debug::Log("GPU has been successfully abstracted!");
      return device;
    }
    catch (vk::SystemError err)
    {
      Debug::Error("Device creation failed!");
      return nullptr;
    }

    return nullptr;
  }

  std::array<vk::Queue, 2> getQueues(vk::PhysicalDevice physical_device,
                                     vk::SurfaceKHR surface, vk::Device device)
  {
    QueueFamilyIndices indices = FindQueueFamilies(physical_device, surface);
    return { { device.getQueue(indices.graphics_family.value(), 0),
               device.getQueue(indices.present_family.value(), 0) } };
  }

  SwapChainSupportDetails QuerySwapchainSupport(vk::PhysicalDevice device,
                                                vk::SurfaceKHR surface)
  {
    SwapChainSupportDetails support;
    support.capabilities = device.getSurfaceCapabilitiesKHR(surface);
    
    Debug::Log("Swapchain can support the following capabilities:");
    Debug::Log("Minimum image count %i", support.capabilities.minImageCount);
    Debug::Log("Maximum image count %i", support.capabilities.maxImageCount);

    Debug::Log("Current extent:");
    Debug::Log("Width: %i", support.capabilities.currentExtent.width);
    Debug::Log("Height: %i", support.capabilities.currentExtent.height);

    Debug::Log("Minimum supported extent:");
    Debug::Log("Width: %i", support.capabilities.minImageExtent.width);
    Debug::Log("Height: %i", support.capabilities.minImageExtent.height);

    Debug::Log("Maximum supported extent:");
    Debug::Log("Width: %i", support.capabilities.maxImageExtent.width);
    Debug::Log("Height: %i", support.capabilities.maxImageExtent.height);

    Debug::Log("Maximum image array layers: %i", support.capabilities.maxImageArrayLayers);

    Debug::Log("Supported transforms:");
    std::vector<std::string> string_list = LogTransformBits(support.capabilities.supportedTransforms);
    for (const std::string& line : string_list) Debug::Log("%s", line.c_str());

    Debug::Log("Current transform:");
    string_list = LogTransformBits(support.capabilities.currentTransform);
    for (const std::string& line : string_list) Debug::Log("%s", line.c_str());

    Debug::Log("Supported alpha operations:");
    string_list = LogAlphaCompositeBits(support.capabilities.supportedCompositeAlpha);
    for (const std::string& line : string_list) Debug::Log("%s", line.c_str());

    Debug::Log("Supported image usage:");
    string_list = LodImageUsageBits(support.capabilities.supportedUsageFlags);
    for (const std::string& line : string_list) Debug::Log("%s", line.c_str());

    support.formats = device.getSurfaceFormatsKHR(surface);
    for (vk::SurfaceFormatKHR supported_format : support.formats)
    {
      Debug::Log("Supported pixel format: %s", vk::to_string(supported_format.format).c_str());
      Debug::Log("Supported color space: %s", vk::to_string(supported_format.colorSpace).c_str());
    }

    support.present_modes = device.getSurfacePresentModesKHR(surface);
    for (vk::PresentModeKHR present_mode : support.present_modes)
      Debug::Log("%s", LogPresentMode(present_mode).c_str());

    return support;
  }

  vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(std::vector<vk::SurfaceFormatKHR> formats)
  {
    for (vk::SurfaceFormatKHR format : formats)
    {
      if (format.format == vk::Format::eB8G8R8A8Unorm &&
          format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        return format;
    }

    return formats[0];
  }

  vk::PresentModeKHR ChooseSwapchainPresentMode(std::vector<vk::PresentModeKHR> present_modes)
  {
    for (vk::PresentModeKHR present_mode : present_modes)
    {
      if (present_mode == vk::PresentModeKHR::eMailbox) return present_mode;
    }

    return vk::PresentModeKHR::eFifo;
  }

  vk::Extent2D ChooseSwapchainExtent(uint32_t width, uint32_t height,
                                     vk::SurfaceCapabilitiesKHR capabilities)
  {
    if (capabilities.currentExtent.width != UINT32_MAX)
      return capabilities.currentExtent;
    else
    {
      vk::Extent2D extent = { width, height };
      
      extent.width = std::min(capabilities.maxImageExtent.width,
                              std::max(capabilities.minImageExtent.width, width));

      extent.height = std::min(capabilities.maxImageExtent.height,
                               std::max(capabilities.minImageExtent.height, height));

      return extent;
    }
  }

  SwapChainBundle CreateSwapchain(vk::Device logical_device,
                                  vk::PhysicalDevice physical_device,
                                  vk::SurfaceKHR surface,
                                  int width, int height)
  {
    SwapChainSupportDetails support = QuerySwapchainSupport(physical_device, surface);
    vk::SurfaceFormatKHR format = ChooseSwapchainSurfaceFormat(support.formats);
    vk::PresentModeKHR present_mode = ChooseSwapchainPresentMode(support.present_modes);
    vk::Extent2D extent = ChooseSwapchainExtent(width, height, support.capabilities);

    uint32_t image_count = std::min(support.capabilities.maxImageCount,
                                    support.capabilities.minImageCount + 1);

    vk::SwapchainCreateInfoKHR create_info = vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(),
                                                                        surface, image_count,
                                                                        format.format,
                                                                        format.colorSpace,
                                                                        extent, 1,
                                                                        vk::ImageUsageFlagBits::eColorAttachment);

    QueueFamilyIndices indices = FindQueueFamilies(physical_device, surface);
    uint32_t queue_family_indices[] = { indices.graphics_family.value(),
                                        indices.present_family.value() };

    if (indices.graphics_family.value() != indices.present_family.value())
    {
      create_info.imageSharingMode = vk::SharingMode::eConcurrent;
      create_info.queueFamilyIndexCount = 2;
      create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
      create_info.imageSharingMode = vk::SharingMode::eExclusive;

    create_info.preTransform = support.capabilities.currentTransform;
    create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = vk::SwapchainKHR(nullptr);

    SwapChainBundle bundle {};
    try { bundle.swapchain = logical_device.createSwapchainKHR(create_info); }
    catch (vk::SystemError err) { Debug::Error("Failed to create a Swapchain!"); }

    bundle.images = logical_device.getSwapchainImagesKHR(bundle.swapchain);
    bundle.format = format.format;
    bundle.extent = extent;

    return bundle;
  }

} // namespace VulkanInit
#endif // DEVICE_HPP
