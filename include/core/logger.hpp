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
            static void SetupDefaultSinks();

    };

};