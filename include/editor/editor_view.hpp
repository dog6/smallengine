#pragma once

namespace sg::Editor
{
    class EditorView
    {
    public:
        virtual ~EditorView() = default;

        void Show() noexcept { m_visible = true; }
        void Hide() noexcept { m_visible = false; }
        void Toggle() noexcept { m_visible = !m_visible; }

        [[nodiscard]] bool IsVisible() const noexcept
        {
            return m_visible;
        }

        virtual void Draw() = 0;

    private:
        bool m_visible = true;
    };
}