workspace "Retro"
architecture "x86_64"
startproject "Sandbox"

configurations {
    "Debug",
    "Release",
}

flags {
    "MultiProcessorCompile"
}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ThirdParty/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/ThirdParty/GLAD/include"
IncludeDir["GLM"] = "%{wks.location}/ThirdParty/GLM"
IncludeDir["SPDLOG"] = "%{wks.location}/ThirdParty/SPDLOG/include"
IncludeDir["IMGUI"] = "%{wks.location}/ThirdParty/IMGUI"
IncludeDir["STB"] = "%{wks.location}/ThirdParty/STB"
IncludeDir["ASSIMP"] = "%{wks.location}/ThirdParty/ASSIMP/include"

group "Dependencies"
    include "ThirdParty/GLFW"
    include "ThirdParty/GLAD"
    include "ThirdParty/IMGUI"
    include "ThirdParty/STB"
group ""

project "EngineCore"
    location "Engine/Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
    objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Engine/Core/Source/pch.cpp"

    files {
        "%{wks.location}/Engine/Core/Source/**.h",
        "%{wks.location}/Engine/Core/Source/**.cpp",
        "%{wks.location}/ThirdParty/STB/**.h",
        "%{wks.location}/ThirdParty/STB/**.cpp",
    }

    libdirs {
        "%{wks.location}/Binaries/Debug-windows-x86_64/IMGUI",
        "%{wks.location}/Binaries/Debug-windows-x86_64/GLFW",
        "%{wks.location}/Binaries/Debug-windows-x86_64/GLAD",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Debug",
    }

    includedirs {
        "%{wks.location}/Engine/Core/Source",
        "%{wks.location}/Engine/Renderer/Source",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.ASSIMP}"
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

    targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
    objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Engine/Renderer/Source/pch.cpp"

    files {
        "%{wks.location}/Engine/Renderer/Source/**.h",
        "%{wks.location}/Engine/Renderer/Source/**.cpp",
        "%{wks.location}/ThirdParty/STB/**.h",
        "%{wks.location}/ThirdParty/STB/**.cpp",
    }
    
    libdirs {
        "%{wks.location}/ThirdParty/ASSIMP/bin/Debug"
    }
    
    includedirs {
        "%{wks.location}/Engine/Renderer/Source",
        "%{wks.location}/Engine/Core/Source",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",    
        "%{IncludeDir.STB}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.ASSIMP}"
    }

    links {
        "EngineCore",
        "GLFW",
        "GLAD",
        "IMGUI",
        "STB",
    }

    filter "system:windows"
        cppdialect "C++latest"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "on"
        optimize "Off"
        links {
            "assimp-vc143-mtd.dll"
        }

    filter "configurations:release"
        optimize "Speed"

    filter "configurations:Dist"
        optimize "Full"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
    objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

    files {
        "%{wks.location}/Sandbox/Source/**.h",
        "%{wks.location}/Sandbox/Source/**.cpp"
    }

    includedirs {
        "%{wks.location}/Engine/Core/Source",
        "%{wks.location}/Engine/Renderer/Source",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.ASSIMP}"
    }

    links {
        "EngineCore",
        "EngineRenderer",
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