workspace "DOOM_Style_Shooter"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}

IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["SDL2"] = "%{LibraryDir.VulkanSDK}/SDL2.lib"
Library["SDL2main"] = "%{LibraryDir.VulkanSDK}/SDL2main.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"


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
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Shooter")
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
			"Logger/third-party/spdlog/include",
			"%{IncludeDir.VulkanSDK}"
        }
        
        links
        {
            "Logger",			
			"%{Library.SDL2}",
			"%{Library.SDL2main}",
			"%{Library.Vulkan}"
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
				("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Shooter")
            }
    
        filter "configurations:Debug"
            defines "DEBUG"
            symbols "On"
    
			links
			{
				"%{Library.ShaderC_Debug}",
				"%{Library.SPIRV_Cross_Debug}",
				"%{Library.SPIRV_Cross_GLSL_Debug}"
			}
				
        filter "configurations:Release"
            defines "RELEASE"
            optimize "On"

			links
			{
				"%{Library.ShaderC_Release}",
				"%{Library.SPIRV_Cross_Release}",
				"%{Library.SPIRV_Cross_GLSL_Release}"
			}
			

project "Shooter"
	location "Shooter"
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
		"Logger/src",
		"Logger/third-party/spdlog/include",
		"%{IncludeDir.VulkanSDK}"
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

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
