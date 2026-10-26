#pragma once

#include "editor/editor_view.hpp"
#include "editor/views/performance_view.hpp"

#include <memory>
#include <vector>

namespace sg::Editor
{
    class EditorUIManager
    {
    public:
        EditorUIManager()
        {
            m_performanceView = std::make_shared<PerformanceView>();
            m_views.push_back(m_performanceView);
        }

        void SetPerformanceStats(const PerformanceStats& stats)
        {
            m_performanceView->SetStats(stats);
        }

        void Draw()
        {
            for (const auto& view : m_views)
            {
                if (view && view->IsVisible())
                    view->Draw();
            }
        }

        void ShowAll()
        {
            for (const auto& view : m_views)
                if (view) view->Show();
        }

        void HideAll()
        {
            for (const auto& view : m_views)
                if (view) view->Hide();
        }

        [[nodiscard]]
        std::shared_ptr<PerformanceView> GetPerformanceView() const
        {
            return m_performanceView;
        }

    private:
        std::vector<std::shared_ptr<EditorView>> m_views;
        std::shared_ptr<PerformanceView> m_performanceView;
    };
}