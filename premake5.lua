workspace "MadSword"
    architecture "x86_64"
    startproject "Sandbox"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

--临时变量 定义 输出目录
--详细的所有支持的tokens
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "MadSword/vendor/spdlog/include"
IncludeDir["glfw"] = "MadSword/vendor/glfw/include"
IncludeDir["glad"] = "MadSword/vendor/glad/include"
IncludeDir["imgui"] = "MadSword/vendor/imgui"
IncludeDir["glm"] = "MadSword/vendor/glm"

include "MadSword/vendor/glfw"
include "MadSword/vendor/glad"
include "MadSword/vendor/imgui"

project "MadSword" --项目名称
    location "MadSword" --相对路径
    kind "StaticLib"    --表明该项目是dll动态库
    language "C++"
    staticruntime "on"
    cppdialect "C++17"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")--输出目录
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")--中间临时文件

    pchheader "mspch.h"
    pchsource "MadSword/src/mspch.cpp"

    files--该项目的文件
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs --包含目录
    {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "glfw",
        "glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows" --windows平台配置
        systemversion "latest"

        defines --预编译宏
        {
            "MS_BUILD_DLL",
            "MS_PLATFORM_WINDOWS",
            "MS_ENABLE_ASSERTS",
            "GLFW_INCLUDE_NONE",
            "_WINDLL",
            "_UNICODE",
            "UNICODE",
        }

        -- postbuildcommands --build后的自定义命令
        -- {
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --拷贝引擎库文件
        -- }

    filter "configurations:Debug"
        defines "MS_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "MS_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MS_RELEASE"
        runtime "Release"
        optimize "on"

project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")--输出目录
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")--中间临时文件

        files--该项目的文件
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/src/**.hpp"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        includedirs --包含目录
        {
            "MadSword/vendor/spdlog/include",
            "MadSword/src",
            "%{prj.name}/src",
            "%{IncludeDir.imgui}",
            "%{IncludeDir.glm}"
        }

        links
        {
            "MadSword"
        }
        
        filter "system:windows" --windows平台配置
            systemversion "latest"

            defines --预编译宏
            {
                "MS_PLATFORM_WINDOWS",
                "_UNICODE",
                "UNICODE",
            }

        filter "configurations:Debug"
            defines "MS_DEBUG"
            runtime "Debug"
            symbols "on"
        
        filter "configurations:Release"
            defines "MS_RELEASE"
            runtime "Release"
            optimize "on"
        
        filter "configurations:Dist"
            defines "MS_DIST"
            runtime "Release"
            optimize "on"
        