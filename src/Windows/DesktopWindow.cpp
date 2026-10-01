#include "Windows/DesktopWindow.hpp"
#include <windows.h>
#include <dwmapi.h>

namespace pp {

DesktopWindow::DesktopWindow(sf::Vector2u size)
    : m_window(sf::VideoMode(size), "PixelPet", sf::Style::None, sf::State::Windowed) {
    m_window.setFramerateLimit(60);
    makeTransparent();
    setAlwaysOnTop(true);
}

void DesktopWindow::makeTransparent() {
    HWND hwnd = m_window.getNativeHandle();
    // Extend the DWM glass into the whole client area so alpha=0 pixels are see-through.
    MARGINS margins{-1, -1, -1, -1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    // No taskbar button, and never steal focus.
    LONG_PTR ex = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, ex | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
}

void DesktopWindow::setAlwaysOnTop(bool enabled) {
    SetWindowPos(m_window.getNativeHandle(), enabled ? HWND_TOPMOST : HWND_NOTOPMOST,
                 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

sf::IntRect DesktopWindow::workArea() {
    RECT r{};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
    return {{r.left, r.top}, {r.right - r.left, r.bottom - r.top}};
}

} // namespace pp
