workspace "DOOM_Style_Shooter"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Logger"
	location "Logger"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/third-party/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10"

		defines
		{
			"LOG_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/App")
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"


project "Engine"
        location "Engine"
        kind "SharedLib"
        language "C++"
    
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
    
        includedirs
        {
            "Logger/src",
			"Logger/third-party/spdlog/include"
        }
        
        links
        {
            "Logger"
        }

        filter "system:windows"
            cppdialect "C++20"
            staticruntime "On"
			systemversion "10"
    
            defines
            {
                "ENGINE_BUILD_DLL"
            }
    
            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/App")
            }
    
        filter "configurations:Debug"
            defines "DEBUG"
            symbols "On"
    
        filter "configurations:Release"
            defines "RELEASE"
            optimize "On"

project "Shooter"
	location "Shooter"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",
		"Logger/src",
		"Logger/third-party/spdlog/include"
	}

	links
	{
		"Engine",
		"Logger"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "10"

		defines
		{
			"SHOOTER_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"

project "App"
	location "App"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",
		"Shooter/src",
		"Logger/src",
		"Logger/third-party/spdlog/include"
	}

	links
	{
		"Shooter",
		"Engine",
		"Logger"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "10"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
