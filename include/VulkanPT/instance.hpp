
#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <VulkanPT/config.hpp>
#include <vector>

namespace VulkanInit
{
  vk::Instance MakeInstance(bool debug, const char* application_name)
  {
    Debug::Log("Creating and instance\n");

    uint32_t version { 0 };
    vkEnumerateInstanceVersion(&version);
 
    Debug::Log("System can support vulkan Variant: %i\n", VK_API_VERSION_VARIANT(version));
    Debug::Log("Major: %i\n", VK_API_VERSION_MAJOR(version));
    Debug::Log("Minor: %i\n", VK_API_VERSION_MINOR(version));
    Debug::Log("Patch: %i\n", VK_API_VERSION_PATCH(version));

    version &= ~(0xFFFU);
    version = VK_MAKE_API_VERSION(0, 1, 0, 0);

    vk::ApplicationInfo app_info = vk::ApplicationInfo(application_name, version,
                                                       "hard way", version, version);

    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    Debug::Log("extensions to be requested:\n");
    for (const char* extension_name : extensions)
      Debug::Log(" %s \n", extension_name);

    vk::InstanceCreateInfo create_info = vk::InstanceCreateInfo(vk::InstanceCreateFlags(),
                                                                &app_info, 0, nullptr,
                                                                static_cast<uint32_t>(extensions.size()),
                                                                extensions.data());

    try { return vk::createInstance(create_info, nullptr); }
    catch (vk::SystemError err)
    {
      Debug::Error("Failed to create Instance!");
      return nullptr;
    }
  }

} // namespace VulkanInit
#endif // INSTANCE_HPP
