#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>
#include <vector>

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class Animation : public Util::GameObject {

public:
    explicit Animation(const std::vector<std::string>& AnimationPaths);

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    void SetLooping(bool looping) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }

    void SetInterval(int interval) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetInterval(interval);
    }

    void Play() {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->Play();
    }

    void SetVisible(bool visible) { m_Visible = visible; }
    bool IsVisible() const { return m_Visible; }

    void SetCurrentFrame(std::size_t index) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetCurrentFrame(index);
    }

    [[nodiscard]] std::size_t GetCurrentFrameIndex() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetCurrentFrameIndex();
    }

    [[nodiscard]] std::size_t GetFrameCount() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetFrameCount();
    }

    [[nodiscard]] int GetInterval() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetInterval();
    }

    

    [[nodiscard]] bool IfAnimationEnds() const;

};

#endif //ANIMATION_HPP