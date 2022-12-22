workspace "Retro"
architecture "x86_64"
startproject "Showcase"

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
IncludeDir["IMGUIBROWSER"] = "%{wks.location}/ThirdParty/IMGUIBROWSER"
IncludeDir["STB"] = "%{wks.location}/ThirdParty/STB"
IncludeDir["ASSIMP"] = "%{wks.location}/ThirdParty/ASSIMP/include"
IncludeDir["ENTT"] = "%{wks.location}/ThirdParty/ENTT/include"
IncludeDir["CEREAL"] = "%{wks.location}/ThirdParty/CEREAL/include"

group "Dependencies"
    include "ThirdParty/GLFW"
    include "ThirdParty/GLAD"
    include "ThirdParty/IMGUI"
    include "ThirdParty/IMGUIBROWSER"
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
    pchsource "Engine/Core/source/pch.cpp"

    files {
        "%{wks.location}/Engine/Core/source/**.h",
        "%{wks.location}/Engine/Core/source/**.cpp",
        "%{wks.location}/ThirdParty/STB/**.h",
        "%{wks.location}/ThirdParty/STB/**.cpp",
    }

    libdirs {
        "%{wks.location}/Binaries/Debug-windows-x86_64/IMGUI",
        "%{wks.location}/Binaries/Debug-windows-x86_64/GLFW",
        "%{wks.location}/Binaries/Debug-windows-x86_64/GLAD",
        "%{wks.location}/ThirdParty/ASSIMP/lib/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/lib/Release",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Release",
    }

    includedirs {
        "%{wks.location}/Engine/Core/source",
        "%{wks.location}/Engine/Renderer/source",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",    
        "%{IncludeDir.STB}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.IMGUIBROWSER}",
        "%{IncludeDir.ASSIMP}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.CEREAL}"
    }

    filter "system:windows"
        cppdialect "C++latest"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "RETRO_ENABLE_ASSERTS","RETRO_DEBUG", "WIN32_LEAN_AND_MEAN" }
        symbols "on"
        optimize "Off"
        links {
            "assimp-vc143-mtd.lib"
        }

    filter "configurations:Release"
        optimize "Speed"
        links {
            "assimp-vc143-mt.lib",
        }

    filter "configurations:Dist"
        optimize "Full"
        links {
            "assimp-vc143-mt.lib",
        }

project "EngineRenderer"
    location "Engine/Renderer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
    objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Engine/Renderer/source/pch.cpp"

    files {
        "%{wks.location}/Engine/Renderer/source/**.h",
        "%{wks.location}/Engine/Renderer/source/**.cpp",
        "%{wks.location}/ThirdParty/STB/**.h",
        "%{wks.location}/ThirdParty/STB/**.cpp",
    }
    
    libdirs {
        "%{wks.location}/ThirdParty/ASSIMP/lib/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/lib/Release",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Release",
    }
    
    includedirs {
        "%{wks.location}/Engine/Renderer/source",
        "%{wks.location}/Engine/Core/source",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",    
        "%{IncludeDir.STB}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.IMGUIBROWSER}",
        "%{IncludeDir.ASSIMP}",  
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.CEREAL}"
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
        defines { "RETRO_ENABLE_ASSERTS","RETRO_DEBUG", "WIN32_LEAN_AND_MEAN" }
        symbols "on"
        optimize "Off"
        links {
            "assimp-vc143-mtd.lib"
        }

    filter "configurations:Release"
        optimize "Speed"
        links {
            "assimp-vc143-mt.lib",
        }

    filter "configurations:Dist"
        optimize "Full"
        links {
            "assimp-vc143-mt.lib",
        }

project "EngineEditor"
    location "Engine/Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
    objdir("Intermediates/" .. outputDir .. "/%{prj.name}")
    
    files {
        "%{wks.location}/Engine/Editor/source/**.h",
        "%{wks.location}/Engine/Editor/source/**.cpp",
    }

    includedirs {
        "%{wks.location}/Engine/Core/source",
        "%{wks.location}/Engine/Renderer/source",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.STB}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.IMGUIBROWSER}",
        "%{IncludeDir.ASSIMP}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.CEREAL}"
    }

    libdirs {
        "%{wks.location}/ThirdParty/ASSIMP/lib/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/lib/Release",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Release",
    }

    links {
        "EngineCore",
        "EngineRenderer",
    }
    
    filter "system:windows"
        staticruntime "off"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines { "RETRO_ENABLE_ASSERTS","RETRO_DEBUG", "WIN32_LEAN_AND_MEAN" }
        symbols "on"
        links {
            "assimp-vc143-mtd.lib"
        }
    
    filter "configurations:Release"
        defines {  "WIN32_LEAN_AND_MEAN" }
        optimize "on"
        links {
            "assimp-vc143-mt.lib",
        }
    
    filter "configurations:Dist"
        defines { "WIN32_LEAN_AND_MEAN" }
        optimize "on"
        links {
            "assimp-vc143-mt.lib",
        }

project "Showcase"
    location "Showcase"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
    objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

    files {
        "%{wks.location}/Showcase/source/**.h",
        "%{wks.location}/Showcase/source/**.cpp"
    }

    includedirs {
        "%{wks.location}/Engine/Core/source",
        "%{wks.location}/Engine/Renderer/source",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",    
        "%{IncludeDir.STB}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.IMGUIBROWSER}",
        "%{IncludeDir.ASSIMP}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.CEREAL}"
    }

    libdirs {
        "%{wks.location}/ThirdParty/ASSIMP/lib/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Debug",
        "%{wks.location}/ThirdParty/ASSIMP/lib/Release",
        "%{wks.location}/ThirdParty/ASSIMP/bin/Release",
    }

    links {
        "EngineCore",
        "EngineRenderer",
    }

    filter "system:windows"
        staticruntime "off"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "RETRO_ENABLE_ASSERTS","RETRO_DEBUG", "WIN32_LEAN_AND_MEAN" }
        symbols "on"
        links {
            "assimp-vc143-mtd.lib"
        }

    filter "configurations:Release"
        defines {  "WIN32_LEAN_AND_MEAN" }
        optimize "on"
        links {
            "assimp-vc143-mt.lib",
        }

    filter "configurations:Dist"
        defines { "WIN32_LEAN_AND_MEAN" }
        optimize "on"
        links {
            "assimp-vc143-mt.lib",
        }