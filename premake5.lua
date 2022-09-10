workspace "Retro"
  architecture "x86_64" 
  startproject "Sandbox"

  configurations
  {
    "Debug",
    "Release",
  }

project "Engine"
  location "Engine"
  kind "SharedLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  targetdir ("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir ("Intermediates/" .. outputDir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Engine/Source/pch.cpp"

  files {
    "%{prj.name}/Source/**.h",
    "%{prj.name}/Source/**.cpp"
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