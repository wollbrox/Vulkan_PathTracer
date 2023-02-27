
vulkan_sdk = os.getenv("VULKAN_SDK")

workspace "Vulkan Path Tracer"
  filename "VulkanPT"
  language "C++"
  cppdialect "C++17"
  startproject "SandBox"
  configurations { "Debug", "Release" }
  platforms { "Win64" }
  systemversion "latest"
  location "../build"
  objdir "../build/obj"
  defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }
  includedirs {
    "../include",
    "../deps/glfw/include",
    "../deps/glm",
    "../deps/glad/include",
    "%{vulkan_sdk}/Include"
  }

  filter "platforms:Win64"
    system "Windows"
    architecture "x86_64"
  
  filter "configurations:Debug"
    defines { "DEBUG" }
    flags { "DebugEnvsInherit" }
    symbols "On"
    targetsuffix "_d"
    targetdir "../bin"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    targetdir "../bin"

project "VulkanPT"
  uuid "34cb9741-9408-4c86-a84b-4386ebad1a99"
  kind "StaticLib"
  location "../build/VulkanPT"
  links { "%{vulkan_sdk}/Lib/vulkan-1.lib" }
  libdirs { "%{vulkan_sdk}/Lib" }
  files {
    -- application
    "../src/**.cpp",
    "../include/**.hpp",
    
    -- glfw
    "../deps/glfw/include/GLFW/*.h",
    "../deps/glfw/src/*.h",
    "../deps/glfw/src/*.c",

    -- glm
    "../deps/glm/glm/**.hpp",
    "../deps/glm/glm/**.h",
    "../deps/glm/glm/**.inl",

    -- glad
    "../deps/glad/include/glad/*.h",

    -- vulkan
    "%{vulkan_sdk}/Include/**.h",
    "%{vulkan_sdk}/Include/**.hpp"
  }

project "SandBox"
  uuid "e854144b-7f26-41e7-85d7-cbbdf5c2c4aa"
  kind "ConsoleApp"
  location "../build/SandBox"
  links { "VulkanPT", "opengl32" }
  files { "../sandbox/*.cpp" }
