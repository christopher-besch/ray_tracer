project "stb"
    language "C"
    architecture "x86_64"
    kind "StaticLib"

    location ""
    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    filter ""

    -- all files that are included or compiled = all files that are visible to the preprocessor/compiler/linker and make and stuff
    files
    {
        "**.h",
        "**.cpp"
    }

    -- for #include paths with ""
    includedirs {
        ""
    }
