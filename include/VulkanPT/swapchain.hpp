
#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include <VulkanPT/config.hpp>
#include <VulkanPT/logging.hpp>
#include <VulkanPT/queue_families.hpp>

namespace VulkanInit
{
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

    VulkanUtils::QueueFamilyIndices indices = VulkanUtils::FindQueueFamilies(physical_device,
                                                                             surface);
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
#endif // SWAPCHAIN_HPP
