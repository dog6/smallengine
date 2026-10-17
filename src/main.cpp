#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output

// #include <core/settings.hpp>
#include "../include/core/settings.hpp"

void setup_spdlog(){
    // 1. Create the console sink (with color support)
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);

    // 2. Create your file sink
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/game.log", true);
    file_sink->set_level(spdlog::level::info);

    // 3. Combine both sinks into a single vector
    std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };

    // 4. Create a multi-sink logger and set it as the global logger
    auto multi_logger = std::make_shared<spdlog::logger>("global_logger", sinks.begin(), sinks.end());
    
    // Register it globally and set the default pointer
    spdlog::register_logger(multi_logger);
    spdlog::set_default_logger(multi_logger);
}

int main()
{

    // Setup logging
    setup_spdlog();

    // Read settings
    Settings::ReadSettings();

    ProgInfo* pi = Settings::GetProgInfo();
    spdlog::info("Printing settings..\nVersion: {}\nStarted on: {}\nAuthor: {}\n", pi->version, pi->started_on, pi->author);


    
}