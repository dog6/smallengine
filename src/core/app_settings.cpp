#include <iostream>
#include <toml++/toml.hpp>
#include "core/app_settings.hpp"
#include <filesystem>
#include <string>
#include <spdlog/spdlog.h>

using namespace sg::Core;

// Read settings from 'settings.toml' file, or use default hard-coded values
int AppSettings::ReadSettings()
{
    spdlog::info("Trying to read 'settings.toml' file..");

    try {

        // 3. Pass the path directly to toml::parse_file
        toml::table tbl = toml::parse_file("settings.toml");

        // 1. Read Program Info (Allocated safely on the stack)
        ProgInfo pi;
        pi.version    = tbl["metadata"]["version"].value_or<std::string>("INDEV");
        pi.author     = tbl["metadata"]["author"].value_or<std::string>("Benjamin Smallarz");
        pi.started_on = tbl["metadata"]["started_on"].value_or<std::string>("2026-10-26");

        // 2. Read Window Settings (Allocated safely on the stack)
        WindowSettings ws;
        ws.title        = tbl["window"]["title"].value_or<std::string>("SmallEngine");
        ws.resolution.x = tbl["window"]["resolution"]["x"].value_or<unsigned int>(1280);
        ws.resolution.y = tbl["window"]["resolution"]["y"].value_or<unsigned int>(720);
        ws.max_framerate = tbl["window"]["max_framerate"].value_or<int>(60);

        // 3. Set static settings objects (Passes copies or references safely)
        AppSettings::SetWindowSettings(ws);
        AppSettings::SetProgInfo(pi);

    } 
    catch (const toml::parse_error& err) {
        spdlog::error("Failed to parse 'settings.toml': {} at ({}, {})", 
                      err.description(), 
                      err.source().begin.line, 
                      err.source().begin.column);

        
        // 1. Read Program Info (Allocated safely on the stack)
        ProgInfo pi;
        pi.version    = "INDEV";
        pi.author     = "Benjamin Smallarz";
        pi.started_on = "10-16-2026";

        // 2. Read Window Settings (Allocated safely on the stack)
        WindowSettings ws;
        ws.title        = "SmallEngine";
        ws.resolution.x = 1280;
        ws.resolution.y = 720;
        ws.max_framerate = 60;

        // 3. Set static settings objects (Passes copies or references safely)
        AppSettings::SetWindowSettings(ws);
        AppSettings::SetProgInfo(pi);

        return -2;
    }

    spdlog::info("Successfully read 'settings.toml' file");
    return 0;
}


// Window Settings
int AppSettings::SetWindowSettings(const WindowSettings &ws) { 
    if (&ws == nullptr) return -1;
    AppSettings::window = ws;
    return 0;
}

struct WindowSettings* AppSettings::GetWindowSettings() { return &AppSettings::window; }

// Program Info
int AppSettings::SetProgInfo(const ProgInfo &pi) { 
    if (&pi == nullptr) return -1;
    AppSettings::proginfo = pi;
    return 0;
}

struct ProgInfo* AppSettings::GetProgInfo() { return &AppSettings::proginfo; }
