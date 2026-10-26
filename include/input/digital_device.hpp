#pragma once

#include <algorithm>
#include <initializer_list>

namespace sg::Input {

template <typename Derived, typename Code>
class DigitalDevice {
public:
    DigitalDevice() = delete;

    static bool IsDown(Code code)
    {
        return Derived::Query(code);
    }

    static bool IsUp(Code code)
    {
        return !IsDown(code);
    }

    static bool AnyDown(std::initializer_list<Code> codes)
    {
        return std::any_of(codes.begin(), codes.end(), &DigitalDevice::IsDown);
    }

    static bool AllDown(std::initializer_list<Code> codes)
    {
        return std::all_of(codes.begin(), codes.end(), &DigitalDevice::IsDown);
    }

    static float Axis(Code negative, Code positive, float scale = 1.0f)
    {
        const int direction = static_cast<int>(IsDown(positive))
                            - static_cast<int>(IsDown(negative));
        return static_cast<float>(direction) * scale;
    }
};

}