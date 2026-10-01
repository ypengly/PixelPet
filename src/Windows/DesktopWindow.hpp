#pragma once
#include <SFML/Graphics.hpp>

namespace pp {

// Borderless, transparent, always-on-top window that hosts the pet.
class DesktopWindow {
public:
    explicit DesktopWindow(sf::Vector2u size);

    sf::RenderWindow& window() { return m_window; }
    void setAlwaysOnTop(bool enabled);
    // Usable desktop area (excludes the taskbar) of the primary monitor.
    static sf::IntRect workArea();

private:
    void makeTransparent();
    sf::RenderWindow m_window;
};

} // namespace pp
