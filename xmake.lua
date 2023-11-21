set_project("HAIEngine")
set_version("1.0.0")

add_rules("mode.debug", "mode.release","plugin.vsxmake.autoupdate")

add_requires("spdlog", "glfw", "glad", "glm", "imgui docking", "stb", "nlohmann_json")

 if is_plat("windows") then
    add_defines("HE_PLATFORM_WINDOWS")
end

if is_mode("debug") then
    add_defines("HE_ENABLE_ASSERTS")
end

target("HAIEngine")
    set_kind("static")
    set_languages("c17","cxx20")
    add_includedirs("src")
    add_files("Src/**.cpp")
    add_packages("spdlog", "glfw", "glad", "glm", "imgui", "stb", "nlohmann_json")
    add_headerfiles("Src/**.h")
    add_headerfiles("Src/**.hpp")
target_end()

-- TODO clean
target("Sandbox")
    set_kind("binary")
    set_languages("c17","cxx20")
    add_files("Sandbox/src/**.cpp")
    add_includedirs("Src")
    add_packages("spdlog", "glm", "imgui", "glad", "glfw", "stb", "nlohmann_json")
    add_deps("HAIEngine")
    add_headerfiles("Sandbox/src/**.h")
    add_headerfiles("Sandbox/src/**.hpp")
target_end()
    
