
#ifndef FRAME_HPP
#define FRAME_HPP

#include <VulkanPT/config.hpp>

namespace VulkanUtils
{
  struct SwapChainFrame
  {
    vk::Image image;
    vk::ImageView image_view;
  };

} // namespace VulkanUtils
#endif // FRAME_HPP
