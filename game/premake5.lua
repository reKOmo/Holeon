
baseName = path.getbasename(os.getcwd());

project (workspaceName)
  	kind "ConsoleApp"
    location "../game"
    targetdir "../bin/%{cfg.buildcfg}"
	objdir "../bin-obj/%{cfg.buildcfg}"

	filter "configurations:Debug"
		defines {
			"HOLO_DEV"
		}
	
    filter "configurations:Release"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"

	filter "action:vs*"
        debugdir "$(SolutionDir)"
		
	filter {"action:vs*", "configurations:Release"}
			kind "WindowedApp"
			entrypoint "mainCRTStartup"
	filter {}
	
	vpaths 
	{
	  ["Header Files/*"] = { "include/**.h",  "include/**.hpp", "src/**.h", "src/**.hpp", "**.h", "**.hpp"},
	  ["Source Files/*"] = {"src/**.c", "src/**.cpp","**.c", "**.cpp"},
	}
	files {"**.c", "**.cpp", "**.h", "**.hpp"}

	includedirs {
		"./",
		"src/**",
		"include",
		"../vendor/entt/include",
		"../vendor/raylib-cpp/include",
		"../vendor/tweeny/include",
	}
	link_raylib();
	
	-- To link to a lib use link_to("LIB_FOLDER_NAME")