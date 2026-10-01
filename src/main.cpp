#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <random>
#include "Windows/DesktopWindow.hpp"
#include "Pet/PetRenderer.hpp"

namespace {
constexpr unsigned kPetSize = 128;
constexpr float kWalkSpeed = 45.f; // px/s
enum class Mode { Idle, Walking };
}

int main() {
    pp::DesktopWindow desktop({kPetSize, kPetSize});
    auto& win = desktop.window();
    pp::PetRenderer renderer;
    std::mt19937 rng{std::random_device{}()};
    auto rnd = [&](float a, float b) { return std::uniform_real_distribution<float>(a, b)(rng); };

    auto area = pp::DesktopWindow::workArea();
    const int floorY = area.position.y + area.size.y - static_cast<int>(kPetSize);
    sf::Vector2f pos{rnd(area.position.x, area.position.x + area.size.x - kPetSize),
                     static_cast<float>(floorY)};
    win.setPosition(sf::Vector2i(pos));

    Mode mode = Mode::Idle;
    float modeTimer = rnd(2.f, 5.f), dir = 1.f, blinkTimer = rnd(2.f, 5.f), blink = 0.f;
    bool dragging = false;
    sf::Vector2i grab;
    sf::Clock clock, life;

    while (win.isOpen()) {
        while (const std::optional ev = win.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) win.close();
            if (const auto* m = ev->getIf<sf::Event::MouseButtonPressed>()) {
                if (m->button == sf::Mouse::Button::Left) {
                    dragging = true;
                    grab = sf::Mouse::getPosition() - win.getPosition();
                } else if (m->button == sf::Mouse::Button::Right) {
                    win.close(); // temporary: context menu arrives in Phase 6
                }
            }
            if (const auto* m = ev->getIf<sf::Event::MouseButtonReleased>())
                if (m->button == sf::Mouse::Button::Left) dragging = false;
        }
        const float dt = std::min(clock.restart().asSeconds(), 0.05f);
        const float t = life.getElapsedTime().asSeconds();

        if (dragging) {
            pos = sf::Vector2f(sf::Mouse::getPosition() - grab);
        } else {
            // Ease back down to the taskbar after being dropped.
            pos.y += (static_cast<float>(floorY) - pos.y) * std::min(1.f, dt * 8.f);
            modeTimer -= dt;
            if (modeTimer <= 0.f) {
                mode = (mode == Mode::Idle && rnd(0, 1) < 0.6f) ? Mode::Walking : Mode::Idle;
                if (mode == Mode::Walking) dir = rnd(0, 1) < 0.5f ? -1.f : 1.f;
                modeTimer = mode == Mode::Walking ? rnd(2.f, 5.f) : rnd(2.f, 6.f);
            }
            if (mode == Mode::Walking) {
                pos.x += dir * kWalkSpeed * dt;
                const float lo = static_cast<float>(area.position.x);
                const float hi = static_cast<float>(area.position.x + area.size.x - kPetSize);
                if (pos.x < lo || pos.x > hi) { pos.x = std::clamp(pos.x, lo, hi); dir = -dir; }
            }
        }
        win.setPosition(sf::Vector2i(pos));

        blinkTimer -= dt;
        if (blinkTimer <= 0.f) { blink = 1.f; blinkTimer = rnd(2.f, 6.f); }
        blink = std::max(0.f, blink - dt * 8.f);

        pp::PetPose pose;
        pose.breathe = std::sin(t * 2.f);
        pose.blink = blink;
        pose.facingRight = dir > 0.f;
        pose.tailSwing = std::sin(t * 3.f);
        if (mode == Mode::Walking && !dragging) pose.bob = -std::abs(std::sin(t * 9.f)) * 4.f;

        win.clear(sf::Color::Transparent);
        renderer.draw(win, pose, static_cast<float>(kPetSize));
        win.display();
    }
}
