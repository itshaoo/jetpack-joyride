#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Util/Animation.hpp"
#include "Util/Renderer.hpp"
#include "Util/BGM.hpp"
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
    void AddToRenderer(Util::Renderer &renderer);
    void Update();
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
    std::shared_ptr<Util::BGM> stepSound;

    std::shared_ptr<GravitySuit> gravitySuit;
    bool hasGravitySuit = false;

    std::shared_ptr<LilStomper> lilStomper;
    bool hasLilStomper = false;
    
    int lastFrameIndex = -1;
    glm::vec2 groundPosition =  { -300.5f, -265.5f };
    float maxHeight = 250.0f;
    float speed = 7.5f;
    float m_Distance = 0.0f;
    PlayerState state = PlayerState::FallingDown; // 初始狀態為掉落動畫
    bool lastSpacePressed = false;  // 上一幀的 space 狀態

    std::shared_ptr<Animation> lastVisibleAnimation = nullptr;
};