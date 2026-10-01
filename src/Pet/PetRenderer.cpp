#include "Pet/PetRenderer.hpp"
#include <cmath>

namespace pp {
namespace {
const sf::Color kFur{247, 236, 226};
const sf::Color kShade{232, 214, 200};
const sf::Color kInner{255, 178, 190};
const sf::Color kInk{58, 44, 60};

sf::CircleShape ellipse(sf::Vector2f c, sf::Vector2f r, sf::Color col) {
    sf::CircleShape s(1.f, 48);
    s.setOrigin({1.f, 1.f});
    s.setScale(r);
    s.setPosition(c);
    s.setFillColor(col);
    return s;
}
sf::ConvexShape tri(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Color col) {
    sf::ConvexShape s(3);
    s.setPoint(0, a); s.setPoint(1, b); s.setPoint(2, c);
    s.setFillColor(col);
    return s;
}
} // namespace

void PetRenderer::draw(sf::RenderTarget& t, const PetPose& p, float px) const {
    const float k = px / 160.f;
    const float dir = p.facingRight ? 1.f : -1.f;
    const float sy = 1.f + 0.025f * p.breathe;
    sf::Transform tf;
    tf.translate({0.f, 160.f * k});            // anchor feet to the bottom
    tf.scale({k, k * sy});
    tf.translate({80.f, 0.f});
    tf.scale({dir, 1.f});
    tf.translate({-80.f, -160.f + p.bob});
    sf::RenderStates rs(tf);

    // Tail
    sf::ConvexShape tail(4);
    float sw = p.tailSwing * 10.f;
    tail.setPoint(0, {58, 132}); tail.setPoint(1, {30 - sw, 118});
    tail.setPoint(2, {22 - sw, 96}); tail.setPoint(3, {34 - sw, 100});
    tail.setFillColor(kShade);
    t.draw(tail, rs);
    t.draw(ellipse({28 - sw, 98}, {7, 7}, kShade), rs);

    // Body + paws
    t.draw(ellipse({80, 124}, {36, 28}, kFur), rs);
    t.draw(ellipse({62, 148}, {12, 8}, kFur), rs);
    t.draw(ellipse({98, 148}, {12, 8}, kFur), rs);

    // Ears
    t.draw(tri({36, 62}, {40, 22}, {70, 42}, kFur), rs);
    t.draw(tri({124, 62}, {120, 22}, {90, 42}, kFur), rs);
    t.draw(tri({44, 54}, {45, 32}, {62, 44}, kInner), rs);
    t.draw(tri({116, 54}, {115, 32}, {98, 44}, kInner), rs);

    // Big head
    t.draw(ellipse({80, 74}, {50, 42}, kFur), rs);

    // Eyes (large, expressive) with blink squash
    const float open = 1.f - 0.92f * p.blink;
    for (float ex : {58.f, 102.f}) {
        t.draw(ellipse({ex, 78}, {10, 12 * open}, kInk), rs);
        if (open > 0.4f) {
            t.draw(ellipse({ex - 3.f, 73}, {3.5f, 3.5f}, sf::Color::White), rs);
            t.draw(ellipse({ex + 3.f, 83}, {1.8f, 1.8f}, sf::Color(255, 255, 255, 190)), rs);
        }
    }
    // Cheeks, nose, mouth
    t.draw(ellipse({40, 92}, {8, 5}, sf::Color(255, 190, 200, 150)), rs);
    t.draw(ellipse({120, 92}, {8, 5}, sf::Color(255, 190, 200, 150)), rs);
    t.draw(tri({76, 90}, {84, 90}, {80, 95}, kInner), rs);
    t.draw(ellipse({75, 99}, {4, 2.2f}, kInk), rs);
    t.draw(ellipse({85, 99}, {4, 2.2f}, kInk), rs);
}

} // namespace pp
