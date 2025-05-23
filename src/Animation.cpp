#include "Animation.hpp"

Animation::Animation(const std::vector<std::string>& AnimationPaths, float zIndex /*=0.0f*/) {
    m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, false, 500, false, 0);
    SetZIndex(zIndex);
}

bool Animation::IfAnimationEnds() const {
    auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}