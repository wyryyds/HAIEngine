set_project("HAIEngine")
set_version("1.0.0")

add_rules("mode.debug", "mode.release","plugin.vsxmake.autoupdate")

add_requires("spdlog","glfw","glad","glm","imgui docking")

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
    add_packages("spdlog","glm","imgui","glad","glfw")
    add_deps("HAIEngine")
    add_headerfiles("Sandbox/src/**.h")

target("HAIEngine")
    set_kind("static")
    set_languages("c17","cxx20")
    add_includedirs("HAIEngine/src")
    add_files("HAIEngine/src/**.cpp")
    add_packages("spdlog","glfw","glad","glm","imgui")
    add_headerfiles("HAIEngine/src/**.h")
