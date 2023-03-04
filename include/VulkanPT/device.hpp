
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
                                                            0, nullptr,
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

} // namespace VulkanInit
#endif // DEVICE_HPP
