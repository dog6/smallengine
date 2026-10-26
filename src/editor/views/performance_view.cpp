#include "editor/views/performance_view.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

namespace sg::Editor
{
    void PerformanceView::Draw()
    {
        if (!IsVisible())
            return;

        ImGui::SetNextWindowBgAlpha(0.85f);

        if (ImGui::Begin("Performance", nullptr,
                         ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("FPS: %6.1f", m_stats.fps);
            ImGui::Text("Entities: %zu", m_stats.entityCount);
            ImGui::Text("Physics step: %.3f ms",
                        m_stats.physicsStepMs);
        }

        ImGui::End();
    }
}