#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <vector>
#include "Animation.hpp"
#include "Util/Renderer.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"
#include "Util/BGM.hpp" 
#include <glm/glm.hpp>

class Player {
public:
    Player();
    // 在每個遊戲更新循環中呼叫，用來更新 Barry 的狀態與位置
    void Update();
    // 將 Barry 的動畫加入到 Renderer 中， Renderer 負責 Draw()
    void AddToRenderer(Util::Renderer &renderer);

    // 取得目前 Barry 的位置（所有動畫共享相同位置）
    glm::vec2 m_Position; // Barry 的位置

    // 設置位置
    void SetPosition(const glm::vec2& position) {
        m_Position = position;
    }
    // 取得目前 Barry 的位置（所有動畫共享相同位置）
    glm::vec2 GetPosition() const;

    float GetDistance() const { return m_Distance; }


private:
    std::shared_ptr<Animation> runAnimation;
    std::shared_ptr<Animation> flyAnimation;
    std::shared_ptr<Animation> fallAnimation;

    std::shared_ptr<Util::BGM> stepSound; // 步伐音效
    const float stepDistance = 7.5f;    

    const glm::vec2 groundPosition = { -300.5f, -265.5f };
    const float maxHeight = 250.0f;
    const float speed = 7.5f;
    float m_Distance = 0.0f; // Tracks the distance covered by the player

    int lastFrameIndex = -1; // Tracks the last frame index for step sound
};

#endif
