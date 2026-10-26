#pragma once

#include "editor/editor_view.hpp"

#include <cstddef>

namespace sg::Editor
{
    struct PerformanceStats
    {
        float fps = 0.0f;
        std::size_t entityCount = 0;
        float physicsStepMs = 0.0f;
    };

    class PerformanceView final : public EditorView
    {
    public:
        void SetStats(const PerformanceStats& stats) noexcept
        {
            m_stats = stats;
        }

        void Draw() override;

    private:
        PerformanceStats m_stats{};
    };
}