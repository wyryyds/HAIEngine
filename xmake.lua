add_rules("mode.debug", "mode.release","plugin.vsxmake.autoupdate")

add_requires("spdlog","glfw","glad","imgui")

 if is_plat("windows") then
    add_defines("HE_PLATFORM_WINDOWS")
end

if is_mode("debug") then
    add_defines("HE_ENABLE_ASSERTS")
end

target("Sandbox")
    set_kind("binary")
    set_languages("c17","cxx20")
    add_files("Sandbox/src/**.cpp")
    add_includedirs("HAIEngine/src")
    add_packages("spdlog","glfw","glad","imgui")
    add_deps("HAIEngine")
    add_headerfiles("Sandbox/src/**.h")

target("HAIEngine")
    set_kind("shared")
    set_languages("c17","cxx20")
    add_includedirs("HAIEngine/src")
    add_files("HAIEngine/src/**.cpp")
    add_packages("spdlog","glfw","glad","imgui")
    add_defines("HE_BUILD_DLL")
    add_headerfiles("HAIEngine/src/**.h")
