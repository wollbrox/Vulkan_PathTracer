
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
} // namespace VulkanInit
#endif // LOGGING_HPP
