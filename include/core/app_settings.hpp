#pragma once
#include "core/setting_types.hpp"

namespace sg::Core {

    class AppSettings {

        private:
            static AppSettings& Instance;
            inline static ProgInfo proginfo;
            inline static WindowSettings window;

            // reads settings
            public:
                static int ReadSettings();
                static int SetWindowSettings(const WindowSettings& ws);
                static WindowSettings* GetWindowSettings();
                static int SetProgInfo(const ProgInfo& pi);
                static ProgInfo* GetProgInfo();
    };

};