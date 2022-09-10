workspace "Retro"
  architecture "x86_64" 
  startproject "Sandbox"

  configurations
  {
    "Debug",
    "Release",
  }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ThirdParty/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/ThirdParty/GLAD/include"
IncludeDir["GLM"] = "%{wks.location}/ThirdParty/GLM"

project "EngineCore"
  location "Engine/Core"
  kind "SharedLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  targetdir ("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir ("Intermediates/" .. outputDir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Engine/Core/Source/pch.cpp"

  files {
    "Engine/Core/Source/**.h",
    "Engine/Core/Source/**.cpp"
  }

  includedirs {
    "%{prj.name}/Source"
  }

  filter "system:windows"
		cppdialect "C++latest"
		systemversion "latest"

  filter "configurations:Debug"
    symbols "on"
    optimize "Off"

  filter "configurations:release"
    optimize "Speed"   
    
  filter "configurations:Dist"
		optimize "Full"

project "EngineRenderer"
  location "Engine/Renderer"
  kind "SharedLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  targetdir ("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir ("Intermediates/" .. outputDir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Engine/Renderer/Source/pch.cpp"

  files {
    "Engine/Renderer/Source/**.h",
    "Engine/Renderer/Source/**.cpp"
  }

  includedirs {
    "%{prj.name}/Source",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.GLM}",
  }

  filter "system:windows"
		cppdialect "C++latest"
		systemversion "latest"

  filter "configurations:Debug"
    symbols "on"
    optimize "Off"

  filter "configurations:release"
    optimize "Speed"   
    
  filter "configurations:Dist"
		optimize "Full"