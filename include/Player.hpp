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
#include "Util/Time.hpp"
#include "GravitySuit.hpp"
#include "LilStomper.hpp"

enum class PlayerState {
    Running,      // 地面行走動畫
    FlyingUp,     // 飛上去動畫
    AtTop,        // 停在最高點，等待按 SPACE 才會掉下來
    FallingDown   // 掉落動畫
};

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

    glm::vec2 GetSize() const {
        return runAnimation->GetScaledSize();
    }

    float GetDistance() const { return m_Distance; }

    void EnableGravitySuit();
    void EnableLilStomper();
    void DisableGravitySuit();
    void DisableLilStomper();
    bool HasGravitySuit() const { return hasGravitySuit; }
    bool HasLilStomper() const { return hasLilStomper; }

private:
    std::shared_ptr<Animation> runAnimation;
    std::shared_ptr<Animation> flyAnimation;
    std::shared_ptr<Animation> fallAnimation;

    std::shared_ptr<GravitySuit> gravitySuit;
    bool hasGravitySuit = false;

    std::shared_ptr<LilStomper> lilStomper;
    bool hasLilStomper = false;

    std::shared_ptr<Util::BGM> stepSound;
    const float stepDistance = 7.5f;

    PlayerState state = PlayerState::FallingDown; // 初始狀態為掉落動畫
    bool lastSpacePressed = false;  // 上一幀的 space 狀態

    std::shared_ptr<Animation> lastVisibleAnimation = nullptr;

    glm::vec2 groundPosition = { -300.5f, -265.5f };
    const float maxHeight = 250.0f;
    const float speed = 7.5f;

    float m_Distance = 0.0f;

    int lastFrameIndex = -1;
};

#endif