// LilStomper.hpp
#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Util/Animation.hpp"
#include "Animation.hpp"

class LilStomper {
public:
    LilStomper();

    std::shared_ptr<Animation> GetRunAnimation();
    std::shared_ptr<Animation> GetFlyAnimation();
    std::shared_ptr<Animation> GetFallAnimation();
    std::shared_ptr<Animation> GetJumpAnimation();
    glm::vec2 GetSize() const {
        return runAnimation->GetScaledSize();
    }

private:
    std::shared_ptr<Animation> runAnimation;
    std::shared_ptr<Animation> flyAnimation;
    std::shared_ptr<Animation> fallAnimation;
    std::shared_ptr<Animation> jumpAnimation;
};