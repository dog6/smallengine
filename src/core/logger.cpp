#include "core/logger.hpp"

using namespace sg::Core;

// void Logger::SetupDefaultSinks()
// {



//     // console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//     // console_sink->set_level(spdlog::level::info);

//     // file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/game.log", true);
//     // file_sink->set_level(spdlog::level::info);
    
//     // std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };

//     // auto multi_logger = std::make_shared<spdlog::logger>("global_logger", sinks.begin(), sinks.end());
    
//     // // Register it globally and set the default pointer
//     // spdlog::register_logger(multi_logger);
//     // spdlog::set_default_logger(multi_logger);

//     // // 1. Establish the global formatting pattern
//     // spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] %v");

//     // // 2. Crucial for Games: Flush instantly on warning or error so logs aren't lost if the app crashes
//     // spdlog::flush_on(spdlog::level::warn);
// }