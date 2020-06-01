workspace "X-Settlers"
	architecture "x64"
	
	configurations { "Debug", "Release" }
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "X-Settlers"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/{%prj.name}")
	objdir ("obj/" .. outputdir .. "/{%prj.name}")
	
	files {
		"src/**.c",
		"src/**.h",
		"src/**.cpp",
		"src/**.hpp"		
	}
	
	includedirs
	{
		"src",
		"thirdparty/glad/include",
		"thirdparty/SDL2/include",
		"thirdparty/glm",
		"thirdparty/spdlog/include",
		"thirdparty/stb",
		"$(VULKAN_SDK)/Include"
	}
	
	links
	{
		"thirdparty/SDL2/lib/x64/SDL2.lib", 
		"thirdparty/SDL2/lib/x64/SDL2main.lib",
		"$(VULKAN_SDK)/lib/vulkan-1.lib",
		"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
	
		