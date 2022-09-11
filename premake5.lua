workspace "Retro"
  architecture "x86_64" 
  startproject "Sandbox"

  configurations
  {
    "Debug",
    "Release",
  }

  flags
	{
		"MultiProcessorCompile"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ThirdParty/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/ThirdParty/GLAD/include"
IncludeDir["GLM"] = "%{wks.location}/ThirdParty/GLM"
IncludeDir["SPDLOG"] = "%{wks.location}/ThirdParty/SPDLOG/include"

group "Dependencies"
  include "ThirdParty/GLFW"
  include "ThirdParty/GLAD"
group ""

project "EngineCore"
  location "Engine/Core"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "off"

  targetdir ("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir ("Intermediates/" .. outputDir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Engine/Core/Source/pch.cpp"

  files {
    "%{wks.location}/Engine/Core/Source/**.h",
    "%{wks.location}/Engine/Core/Source/**.cpp"
  }

  includedirs {
    "%{wks.location}/Engine/Core/Source",
    "%{wks.location}/Engine/Renderer/Source",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.GLM}",  
    "%{IncludeDir.SPDLOG}"  
  }

  links {
    "EngineRenderer"
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
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "off"

  targetdir ("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir ("Intermediates/" .. outputDir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Engine/Renderer/Source/pch.cpp"

  files {
    "%{wks.location}/Engine/Renderer/Source/**.h",
    "%{wks.location}/Engine/Renderer/Source/**.cpp"
  }

  includedirs {
    "%{wks.location}/Engine/Renderer/Source",
    "%{wks.location}/Engine/Core/Source",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.GLM}",  
    "%{IncludeDir.SPDLOG}"  
  }

  links {
    "GLFW",
    "GLAD",
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

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "off"

  targetdir ("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir ("Intermediates/" .. outputDir .. "/%{prj.name}")

  files {
    "%{wks.location}/Sandbox/Source/**.h",
    "%{wks.location}/Sandbox/Source/**.cpp"
  }

  includedirs {
    "%{wks.location}/Engine/Core/Source",
    "%{wks.location}/Engine/Renderer/Source",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.GLM}",  
    "%{IncludeDir.SPDLOG}",  
  }

  links {
    "EngineCore",
    "EngineRenderer"
  }

  filter "system:windows"
    staticruntime "off"
    systemversion "latest"

  filter "configurations:Debug"
    defines { "MECHA_DEBUG", "WIN32_LEAN_AND_MEAN" }
    symbols "on"
  
  filter "configurations:release"
    defines { "MECHA_RELEASE", "WIN32_LEAN_AND_MEAN" }
    optimize "on"	
  
  filter "configurations:Dist"
    defines { "MECHA_DIST", "WIN32_LEAN_AND_MEAN" }
    optimize "on"