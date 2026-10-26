#pragma once

// Static helper class to setup spdlog
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> 
#include <spdlog/sinks/basic_file_sink.h>

namespace sg::Core {

    class Logger {
            static inline std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
            static inline std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink;
        public: 
            static void SetupDefaultSinks() {

                static bool initialized = []() {
                    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                    console_sink->set_level(spdlog::level::info);

                    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/game.log", true);
                    file_sink->set_level(spdlog::level::info);
                    
                    std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };
                    auto multi_logger = std::make_shared<spdlog::logger>("global_logger", sinks.begin(), sinks.end());
                    
                    spdlog::register_logger(multi_logger);
                    spdlog::set_default_logger(multi_logger);
                    spdlog::set_level(spdlog::level::info);
                    spdlog::flush_on(spdlog::level::info);
                    return true;
                }();
                
            }
    };

};