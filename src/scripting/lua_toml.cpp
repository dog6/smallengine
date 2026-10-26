#include "scripting/lua_toml.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <toml++/toml.hpp>

int sg::Scripting::LuaTOML::GetPathsFromTOML()
{
    // Read toml file
    spdlog::info("Trying to read '{}' file..", this->toml_path);

    try {

        // // 1. Get the current working directory as a native path object
        // std::filesystem::path wd = std::filesystem::current_path();
        // spdlog::info("Current Working Directory: {}", wd.string());

        // // 2. Safely append the filename using the path separator operator (/)
        // std::filesystem::path settings_path = wd / "/res/scripts/configs/";

        // 3. Pass the path directly to toml::parse_file
        toml::table config = toml::parse_file(this->toml_path);

        // Read scripts array
        toml::array* script_array = config["scripts"].as_array();

        if (script_array == nullptr) {
            spdlog::warn("No scripts found in LuaTOML '{}'", this->toml_path);
            return -1;
        } 
        // else

        for (toml::node& script : *script_array){

            toml::table* tbl = script.as_table();

            if (tbl == nullptr) continue; // skip

            std::string path = (*tbl)["path"].value_or("");
            this->script_paths.emplace_back(path);
            spdlog::info("Found script {} to load from LuaTOML {}", path, this->toml_path);
        }

    } 
    catch (const toml::parse_error& err) {
        spdlog::error("Failed to parse '{}': {} at ({}, {})", 
                    this->toml_path,
                    err.description(), 
                    err.source().begin.line, 
                    err.source().begin.column
        );

        return -2;
    }

    spdlog::info("Successfully read 'settings.toml' file");
    return 0;
}

sg::Scripting::LuaTOML::LuaTOML(std::string toml_path)
{
    this->toml_path = toml_path;
    this->script_paths = std::vector<std::string>();
    GetPathsFromTOML();
}

sg::Scripting::LuaTOML::~LuaTOML()
{
    this->script_paths.clear();
}
