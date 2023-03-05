
#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <VulkanPT/config.hpp>

namespace VulkanInit
{
  VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                               VkDebugUtilsMessageTypeFlagsEXT message_type,
                                               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                               void* pUserData)
  {
    Debug::Error("Validation layer: %s", pCallbackData->pMessage);
    return VK_FALSE;
  }

  vk::DebugUtilsMessengerEXT MakeDebugMessenger(vk::Instance& in_instance,
                                                vk::DispatchLoaderDynamic& in_distach_loader)
  {
    vk::DebugUtilsMessengerCreateInfoEXT create_info = vk::DebugUtilsMessengerCreateInfoEXT(
                                                vk::DebugUtilsMessengerCreateFlagsEXT(),
                                                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                                                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                                                debugCallback, nullptr);

    return in_instance.createDebugUtilsMessengerEXT(create_info, nullptr, in_distach_loader);
  }

  std::vector<std::string> LogTransformBits(vk::SurfaceTransformFlagsKHR bits)
  {
    std::vector<std::string> result;

    if (bits & vk::SurfaceTransformFlagBitsKHR::eIdentity) result.emplace_back("identity");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate90) result.emplace_back("90 degree rotation");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate180) result.emplace_back("180 degree rotation");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate270) result.emplace_back("270 degree rotation");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror) result.emplace_back("horizontal mirror");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) result.emplace_back("horizontal mirror, 90 degree rotation");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) result.emplace_back("horizontal mirror, 180 degree rotation");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) result.emplace_back("horizontal mirror, 270 degree rotation");
    if (bits & vk::SurfaceTransformFlagBitsKHR::eInherit) result.emplace_back("inherited");

    return result;
  }

  std::vector<std::string> LogAlphaCompositeBits(vk::CompositeAlphaFlagsKHR bits)
  {
    std::vector<std::string> result;

    if (bits & vk::CompositeAlphaFlagBitsKHR::eOpaque) result.emplace_back("opaque (alpha ignored)");
    if (bits & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) result.emplace_back("pre multiplied (alpha expected to already be multiplied in image)");
    if (bits & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) result.emplace_back("post multiplied (alpha will be applied during composition)");
    if (bits & vk::CompositeAlphaFlagBitsKHR::eInherit) result.emplace_back("inherit");

    return result;
  }

  std::vector<std::string> LodImageUsageBits(vk::ImageUsageFlags bits)
  {
    std::vector<std::string> result;

    if (bits & vk::ImageUsageFlagBits::eTransferSrc) result.emplace_back("transfer source");
    if (bits & vk::ImageUsageFlagBits::eTransferDst) result.emplace_back("transfer destination");
    if (bits & vk::ImageUsageFlagBits::eSampled) result.emplace_back("sampled");
    if (bits & vk::ImageUsageFlagBits::eStorage) result.emplace_back("storage");
    if (bits & vk::ImageUsageFlagBits::eColorAttachment) result.emplace_back("color attachment");
    if (bits & vk::ImageUsageFlagBits::eDepthStencilAttachment) result.emplace_back("depth stencil attachment");
    if (bits & vk::ImageUsageFlagBits::eTransientAttachment) result.emplace_back("transient attachment");
    if (bits & vk::ImageUsageFlagBits::eInputAttachment) result.emplace_back("input attachment");
    if (bits & vk::ImageUsageFlagBits::eFragmentDensityMapEXT) result.emplace_back("fragment density map");
    if (bits & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR) result.emplace_back("fragment shading rate attachment");

    return result;
  }

  std::string LogPresentMode(vk::PresentModeKHR present_mode)
  {
    switch (present_mode)
    {
      case vk::PresentModeKHR::eImmediate: return "immediate"; break;
      case vk::PresentModeKHR::eMailbox: return "mailbox"; break;
      case vk::PresentModeKHR::eFifo: return "fifo"; break;
      case vk::PresentModeKHR::eFifoRelaxed: return "relaxed fifo"; break;
      case vk::PresentModeKHR::eSharedDemandRefresh: return "shared demand refresh"; break;
      case vk::PresentModeKHR::eSharedContinuousRefresh: return "shared continuous refresh"; break;
      default: return "none/undefined"; break;
    }
  }

} // namespace VulkanInit
#endif // LOGGING_HPP
