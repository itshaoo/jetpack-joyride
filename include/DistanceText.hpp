#ifndef DISTANCETEXT_HPP
#define DISTANCETEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

class DistanceText : public Util::GameObject {
public:
    DistanceText()
        : GameObject(
              std::make_unique<Util::Text>(
                  RESOURCE_DIR "/font/game_font.ttf", 30,
                  "0M", Util::Color::FromName(Util::Colors::WHITE)),
              10) // Z 值
    {
        m_Transform.translation = {-500.0f,300.0f}; // 固定的初始位置
        SetWorldObject(false); // 這個物件不需要在世界座標系中
    }

    std::shared_ptr<Util::Text> GetText() const {
        return std::dynamic_pointer_cast<Util::Text>(m_Drawable);
    }

    void UpdateDistance(float distance) {
        auto text = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        if (text) {
            text->SetText(std::to_string(static_cast<int>(distance)) + "m");
        }
    }

};

#endif // DISTANCETEXT_HPP
