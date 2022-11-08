local BGFX_DIR  = "core/external/bgfx"
local BIMG_DIR  = "core/external/bimg"
local BX_DIR    = "core/external/bx"
local GLFW_DIR   = "core/external/glfw"
local IMGUI_DIR = "core/external/imgui"
local GLM_DIR   = "core/external/glm"
local CORE_DIR  = "core"
local PROJ_DIR  = "project"

workspace "project"
	startproject "project"
	configurations { "Release", "Debug" }
	
	if os.is64bit() and not os.istarget("windows") then
		platforms "x86_64"
	else
		platforms { "x86", "x86_64" }
	end
	
	defines
	{
		-- You can force an API by adding a config define like this
		--"BGFX_CONFIG_RENDERER_AGC"     
		
		--"BGFX_CONFIG_RENDERER_DIRECT3D9"  
		--"BGFX_CONFIG_RENDERER_DIRECT3D11" 
		--"BGFX_CONFIG_RENDERER_DIRECT3D12" 
		--"BGFX_CONFIG_RENDERER_GNM"        
		--"BGFX_CONFIG_RENDERER_METAL"      
		--"BGFX_CONFIG_RENDERER_NVN"        
		--"BGFX_CONFIG_RENDERER_OPENGL"     
		--"BGFX_CONFIG_RENDERER_OPENGLES"
		--"BGFX_CONFIG_RENDERER_VULKAN"  
		--"BGFX_CONFIG_RENDERER_WEBGPU"
	}
	
	filter "platforms:x86"
		architecture "x86"
		
	filter "platforms:x86_64"
		architecture "x86_64"
		
	filter "system:macosx"
		xcodebuildsettings {
			["MACOSX_DEPLOYMENT_TARGET"] = "10.9",
			["ALWAYS_SEARCH_USER_PATHS"] = "YES", -- This is the minimum version of macos we'll be able to run on
		};
		
	filter "configurations:Release"
		defines
		{
			"NDEBUG",
			"BX_CONFIG_DEBUG=0"
		}
		optimize "Full"
	filter "configurations:Debug*"
		defines
		{
			"_DEBUG",
			"BX_CONFIG_DEBUG=1"
		}
		optimize "Debug"
		symbols "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	project "bgfx"
	project "bimg"
	project "bx"
	project "glfw"
	project "imgui"
group ""

function setBxCompat()
	filter "action:vs*"
		includedirs { path.join(BX_DIR, "include/compat/msvc") }
	filter { "system:windows", "action:gmake" }
		includedirs { path.join(BX_DIR, "include/compat/mingw") }
	filter { "system:macosx" }
		includedirs { path.join(BX_DIR, "include/compat/osx") }
		buildoptions { "-x objective-c++" }
end

-- Project
project "project"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	location (PROJ_DIR)
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines "_CRT_SECURE_NO_WARNINGS"
	
	files 
	{
		path.join(PROJ_DIR, "include/**.h"),
		path.join(PROJ_DIR, "include/**.hpp"),
		path.join(PROJ_DIR, "src/**.cpp"),
	}
	
	includedirs
	{
		-- Externals
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(GLFW_DIR, "include"),
		IMGUI_DIR, 
		GLM_DIR,
		
		-- Core
		path.join(CORE_DIR, "include"),
		
		-- Project
		path.join(PROJ_DIR, "include"),
		
	}
	links { "core" }

	setBxCompat()

-- Core
project "core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	location (CORE_DIR)
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "crpch.hpp"
	pchsource (path.join(CORE_DIR, "src/crpch.cpp"))

	files 
	{
		path.join(CORE_DIR, "include/**.h"),
		path.join(CORE_DIR, "include/**.hpp"),
		path.join(CORE_DIR, "src/**.cpp"),
	}
	
	includedirs
	{
		-- Externals
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(GLFW_DIR, "include"),
		IMGUI_DIR, 
		GLM_DIR,
		
		-- Core
		path.join(CORE_DIR, "include"),
	}
	links { "bgfx", "bimg", "bx", "glfw", "imgui", }
	filter "system:windows"
		links { "gdi32", "kernel32", "psapi" }
	filter "system:linux"
		links { "dl", "GL", "pthread", "X11" }
	filter "system:macosx"
		links { "QuartzCore.framework", "Metal.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
	setBxCompat()

-- Externals
project "bgfx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	location (BGFX_DIR)
	
	files
	{
		path.join(BGFX_DIR, "include/bgfx/**.h"),
		path.join(BGFX_DIR, "src/*.cpp"),
		path.join(BGFX_DIR, "src/*.h"),
	}
	excludes
	{
		path.join(BGFX_DIR, "src/amalgamated.cpp"),
	}
	includedirs
	{
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(BGFX_DIR, "include"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(BGFX_DIR, "3rdparty/dxsdk/include"),
		path.join(BGFX_DIR, "3rdparty/khronos"),
		"include",
	}
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		excludes
		{
			path.join(BGFX_DIR, "src/glcontext_glx.cpp"),
			path.join(BGFX_DIR, "src/glcontext_egl.cpp")
		}
	filter "system:macosx"
		files
		{
			path.join(BGFX_DIR, "src/*.mm"),
		}
	setBxCompat()

project "bimg"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	location (BIMG_DIR)
	
	files
	{
		path.join(BIMG_DIR, "include/bimg/*.h"),
		path.join(BIMG_DIR, "src/image.cpp"),
		path.join(BIMG_DIR, "src/image_gnf.cpp"),
		path.join(BIMG_DIR, "src/*.h"),
		path.join(BIMG_DIR, "3rdparty/astc-codec/src/decoder/*.cc")
	}
	includedirs
	{
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(BIMG_DIR, "3rdparty/astc-codec"),
		path.join(BIMG_DIR, "3rdparty/astc-codec/include"),
	}
	setBxCompat()

project "bx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	location (BX_DIR)
	
	files
	{
		path.join(BX_DIR, "include/bx/*.h"),
		path.join(BX_DIR, "include/bx/inline/*.inl"),
		path.join(BX_DIR, "src/*.cpp")
	}
	excludes
	{
		path.join(BX_DIR, "src/amalgamated.cpp"),
		path.join(BX_DIR, "src/crtnone.cpp")
	}
	includedirs
	{
		path.join(BX_DIR, "3rdparty"),
		path.join(BX_DIR, "include")
	}
	filter "configurations:Release"
		defines "BX_CONFIG_DEBUG=0"
	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
	setBxCompat()
		
project "glfw"
	kind "StaticLib"
	language "C"
	location (GLFW_DIR)
	
	--targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	--objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		path.join(GLFW_DIR, "include/GLFW/*.h"),
		path.join(GLFW_DIR, "src/context.c"),
		path.join(GLFW_DIR, "src/egl_context.*"),
		path.join(GLFW_DIR, "src/init.c"),
		path.join(GLFW_DIR, "src/input.c"),
		path.join(GLFW_DIR, "src/internal.h"),
		path.join(GLFW_DIR, "src/monitor.c"),
		path.join(GLFW_DIR, "src/null*.*"),
		path.join(GLFW_DIR, "src/osmesa_context.*"),
		path.join(GLFW_DIR, "src/platform.c"),
		path.join(GLFW_DIR, "src/vulkan.c"),
		path.join(GLFW_DIR, "src/window.c"),
	}
	includedirs { path.join(GLFW_DIR, "include") }
	filter "system:windows"
		defines "_GLFW_WIN32"
		files
		{
			path.join(GLFW_DIR, "src/win32_*.*"),
			path.join(GLFW_DIR, "src/wgl_context.*")
		}
	filter "system:linux"
		defines "_GLFW_X11"
		files
		{
			path.join(GLFW_DIR, "src/glx_context.*"),
			path.join(GLFW_DIR, "src/linux*.*"),
			path.join(GLFW_DIR, "src/posix*.*"),
			path.join(GLFW_DIR, "src/x11*.*"),
			path.join(GLFW_DIR, "src/xkb*.*")
		}
	filter "system:macosx"
		defines "_GLFW_COCOA"
		files
		{
			path.join(GLFW_DIR, "src/cocoa_*.*"),
			path.join(GLFW_DIR, "src/posix_thread.h"),
			path.join(GLFW_DIR, "src/nsgl_context.h"),
			path.join(GLFW_DIR, "src/egl_context.h"),
			path.join(GLFW_DIR, "src/osmesa_context.h"),

			path.join(GLFW_DIR, "src/posix_thread.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/egl_context.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/osmesa_context.c"),                       
		}

	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		
project "imgui"
	kind "StaticLib"
	language "C++"
	location (IMGUI_DIR)
	
	files
	{
		path.join(IMGUI_DIR, "imconfig.h"),
		path.join(IMGUI_DIR, "imgui.h"),
		path.join(IMGUI_DIR, "imgui.cpp"),
		path.join(IMGUI_DIR, "imgui_draw.cpp"),
		path.join(IMGUI_DIR, "imgui_internal.h"),
		path.join(IMGUI_DIR, "imgui_widgets.cpp"),
		path.join(IMGUI_DIR, "imstb_rectpack.h"),
		path.join(IMGUI_DIR, "imstb_textedit.h"),
		path.join(IMGUI_DIR, "imstb_truetype.h"),
		path.join(IMGUI_DIR, "imgui_demo.cpp"),
	}

	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
