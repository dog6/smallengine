#pragma once

#include <algorithm>
#include <initializer_list>
#include <unordered_set>

namespace sg::Input {

template <typename Derived, typename Code>
class DigitalDevice {

private:
    inline static std::unordered_set<Code> held_;
    inline static std::unordered_set<Code> pressed_;
    inline static std::unordered_set<Code> released_;

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

    static bool IsPressed(Code code)
    {
        return pressed_.find(code) != pressed_.end();
    }

    static bool IsReleased(Code code)
    {
        return released_.find(code) != released_.end();
    }

    // Call once at the start of each frame, before polling events.
    static void BeginFrame()
    {
        pressed_.clear();
        released_.clear();
    }

    // Call when the corresponding SFML event arrives.
    static void OnPressed(Code code)
    {
        // Ignore key-repeat events while the input is already held.
        if (held_.insert(code).second)
            pressed_.insert(code);
    }

    static void OnReleased(Code code)
    {
        if (held_.erase(code) > 0)
            released_.insert(code);
    }

    // Prevent stuck inputs if the window loses focus.
    static void ResetEventState()
    {
        held_.clear();
        pressed_.clear();
        released_.clear();
    }
};

}