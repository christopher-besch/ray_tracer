outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "ray_tracer"
    startproject "ray_tracer"

    configurations { "Debug", "Release", "Dist" }

    -- when "IDE" defined, program reads form cwd instead of execution location
    filter "configurations:Debug"
        defines { "DEBUG", "IDE" }
        symbols "On"
    filter "configurations:Release"
        defines { "RELEASE", "IDE" }
        symbols "On"
        optimize "On"
    filter "configurations:Dist"
        defines { "DIST" }
        optimize "On"
    
    filter "system:linux"
        defines { "LINUX" }
    filter "system:windows"
        defines { "WINDOWS" }

    -- cwd
    debugdir ""

project "ray_tracer"
    language "C++"
    architecture "x86_64"
    cppdialect "C++14"
    kind "ConsoleApp"

    location "%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- for #include with ""
    includedirs {
        "%{prj.name}/src",
        "utils/src"
    }

    -- for #include with <>
    sysincludedirs {
        "vendor/stb"
    }

    links {
        "utils",
        "stb"
    }

project "scene_generator"
    language "C++"
    architecture "x86_64"
    cppdialect "C++14"
    kind "ConsoleApp"

    location "%{prj.name}"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- for #include with ""
    includedirs {
        "%{prj.name}/src",
        "utils/src"
    }

    links {
        "utils"
    }

-- dependencies
include "utils"
include "vendor/stb"

