
#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <VulkanPT/config.hpp>
#include <vector>

namespace VulkanInit
{
  bool Supported(std::vector<const char*>& in_extensions,
                 std::vector<const char*>& in_layers)
  {
    // check extensions support
    std::vector<vk::ExtensionProperties> supported_extensions = vk::enumerateInstanceExtensionProperties();
    Debug::Log("This device can support the following extensions:");
    for (vk::ExtensionProperties supported_extension : supported_extensions)
      Debug::Log("%s", supported_extension.extensionName);

    bool found;
    for (const char* extension : in_extensions)
    {
      found = false;
      for (vk::ExtensionProperties supported_extension : supported_extensions)
      {
        if (strcmp(extension, supported_extension.extensionName) == 0)
        {
          found = true;
          Debug::Log("Extension %s is supported!", extension);
        }
      }
      if (!found)
      {
        Debug::Log("Extension %s is not supported!", extension);
        return false;
      }
    }

    // check layers support
    std::vector<vk::LayerProperties> supported_layers = vk::enumerateInstanceLayerProperties();
    Debug::Log("This device can support the following layers:");
    for (vk::LayerProperties supported_layer : supported_layers)
      Debug::Log("%s", supported_layer.layerName);

    for (const char* layer : in_layers)
    {
      found = false;
      for (vk::LayerProperties supported_layer : supported_layers)
      {
        if (strcmp(layer, supported_layer.layerName) == 0)
        {
          found = true;
          Debug::Log("Layer %s is supported!", layer);
        }
      }
      if (!found)
      {
        Debug::Log("Layer %s is not supported!", layer);
        return false;
      }
    }

    return true;
  }

  vk::Instance MakeInstance(const char* application_name)
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

#ifdef DEBUG
    extensions.push_back("VK_EXT_debug_utils");
#endif // DEBUG

    Debug::Log("extensions to be requested:\n");
    for (const char* extension_name : extensions)
      Debug::Log(" %s \n", extension_name);

    std::vector<const char*> layers;

#ifdef DEBUG
    layers.push_back("VK_LAYER_KHRONOS_validation");
#endif //DEBUG

    if (!Supported(extensions, layers)) return nullptr;

    vk::InstanceCreateInfo create_info = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &app_info,
                                                                static_cast<uint32_t>(layers.size()),
                                                                layers.data(),
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
