
solution("Vulkan Path Tracer")
  language ("C++")
  configurations { "Debug", "Release" }
  platforms "x64"
  includedirs {
    "../include",
    "../deps/glfw/include",
    "../deps/glm",
    "../deps/vulkan/include"
  }
  libdirs { "../build/**" }
  location "../build"
  objdir "../build/obj"
  flags { "Cpp17" }
  startproject "SandBox"
  defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }

configuration "vs2022"
  windowstargetplatformversion "10.0"

configuration "Debug"
  defines { "DEBUG" }
  flags { "DebugEnvsInherit", "Symbols", "FullSymbols" }
  targetdir "../bin"
  targetsuffix "_d"

configuration "Release"
  flags { "Optimize" }
  targetdir "../bin"

project ("VulkanPathTracer")
  uuid "34cb9741-9408-4c86-a84b-4386ebad1a99"
  kind "StaticLib"
  files {
    -- application
    "../src/**.cpp",
    "../include/**.hpp",
    
    -- glfw
    "../deps/glfw/include/GLFW/*.h", 
    "../deps/glfw/src/glfw_config.h",
    "../deps/glfw/src/context.c",
    "../deps/glfw/src/init.c",
    "../deps/glfw/src/input.c",
    "../deps/glfw/src/monitor.c",
    "../deps/glfw/src/vulkan.c",
    "../deps/glfw/src/window.c",
    "../deps/glfw/src/win32_platform.h",
    "../deps/glfw/src/win32_joystick.h",
    "../deps/glfw/src/wgl_context.h",
    "../deps/glfw/src/egl_context.h",
    "../deps/glfw/src/osmesa_context.h",
    "../deps/glfw/src/win32_init.c",
    "../deps/glfw/src/win32_joystick.c",
    "../deps/glfw/src/win32_monitor.c",
    "../deps/glfw/src/win32_time.c",
    "../deps/glfw/src/win32_thread.c",
    "../deps/glfw/src/win32_window.c",
    "../deps/glfw/src/wgl_context.c",
    "../deps/glfw/src/egl_context.c",
    "../deps/glfw/src/osmesa_context.c",

    -- glm
    "../deps/glm/glm/**.hpp",
    "../deps/glm/glm/**.h",
    "../deps/glm/glm/**.inl",

    -- vulkan
    "../deps/vulkan/include/**.h",
    "../deps/vulkan/include/**.hpp",
    "../deps/vulkan/include/**.hpp11",
    "../deps/vulkan/include/**.json",
    "../deps/vulkan/include/**.lua",
    "../deps/vulkan/include/**.py"
  }
  configuration "Debug"
    targetdir "../bin/debug"
  configuration "Release"
    targetdir "../bin/release"

project ("SandBox")
  uuid "e854144b-7f26-41e7-85d7-cbbdf5c2c4aa"
  kind "ConsoleApp"
  files { "../sandbox/*.cpp" }
  libdirs { "../deps/vulkan/lib" }
  links { "VulkanPathTracer", "vulkan-1" }
  configuration "Debug"
    targetdir "../bin/debug"
  configuration "Release"
    targetdir "../bin/release"
