#pragma once
#include <SFML/Graphics.hpp>

namespace pp {

// Pose parameters an animation system will drive later (Phase 2).
struct PetPose {
    float breathe   = 0.f;   // -1..1
    float bob       = 0.f;   // vertical walk bounce, px in 160-unit space
    float blink     = 0.f;   // 0 open .. 1 closed
    bool  facingRight = true;
    float tailSwing = 0.f;   // -1..1
};

// Placeholder art drawn from primitives. Replace with sprite sheets later;
// callers only depend on draw(), so the swap is isolated here.
class PetRenderer {
public:
    void draw(sf::RenderTarget& target, const PetPose& pose, float pixelSize) const;
};

} // namespace pp
