
#ifndef QUEUE_FAMILIES_HPP
#define QUEUE_FAMILIES_HPP

#include <VulkanPT/config.hpp>

namespace VulkanUtils
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool IsComplete()
    { return graphics_family.has_value() && present_family.has_value(); }
  };

  QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device,
                                       vk::SurfaceKHR surface)
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

} // namespace VulkanUtils
#endif // QUEUE_FAMILIES_HPP
