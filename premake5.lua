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
IncludeDir["glfw"] = "MadSword/vendor/glfw/include"
IncludeDir["glad"] = "MadSword/vendor/glad/include"
IncludeDir["imgui"] = "MadSword/vendor/imgui"

include "MadSword/vendor/glfw"
include "MadSword/vendor/glad"
include "MadSword/vendor/imgui"

project "MadSword" --项目名称
    location "MadSword" --相对路径
    kind "SharedLib"    --表明该项目是dll动态库
    language "c++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")--输出目录
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")--中间临时文件

    pchheader "mspch.h"
    pchsource "MadSword/src/mspch.cpp"

    files--该项目的文件
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs --包含目录
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/glfw/include",
        "%{prj.name}/vendor/glad/include",
        "%{IncludeDir.imgui}"
    }

    links
    {
        "glfw",
        "glad",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows" --windows平台配置
        cppdialect "c++17"
        staticruntime "On"
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

        postbuildcommands --build后的自定义命令
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --拷贝引擎库文件
        }

    filter "configurations:Debug"
        defines "MS_DEBUG"
        runtime "Debug"
        buildoptions "/MDd"
        symbols "on"
    
    filter "configurations:Release"
        defines "MS_RELEASE"
        runtime "Release"
        buildoptions "/MD"
        optimize "on"

    filter "configurations:Dist"
        defines "MS_RELEASE"
        runtime "Release"
        buildoptions "/MD"
        optimize "on"

project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "c++"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")--输出目录
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")--中间临时文件

        files--该项目的文件
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs --包含目录
        {
            "MadSword/vendor/spdlog/include",
            "MadSword/src"
        }

        links
        {
            "MadSword"
        }
        
        filter "system:windows" --windows平台配置
            cppdialect "c++17"
            staticruntime "On"
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
            buildoptions "/MDd"
            symbols "on"
        
        filter "configurations:Release"
            defines "MS_RELEASE"
            runtime "Release"
            buildoptions "/MD"
            optimize "on"
        
        filter "configurations:Dist"
            defines "MS_DIST"
            runtime "Release"
            buildoptions "/MD"
            optimize "on"
        