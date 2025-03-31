#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <vector>
#include "Animation.hpp"
#include "Util/Renderer.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"
#include <glm/glm.hpp>

class Player {
public:
    Player();
    // 在每個遊戲更新循環中呼叫，用來更新 Barry 的狀態與位置
    void Update();
    // 將 Barry 的動畫加入到 Renderer 中， Renderer 負責 Draw()
    void AddToRenderer(Util::Renderer &renderer);
    // 取得目前 Barry 的位置（所有動畫共享相同位置）
    glm::vec2 GetPosition() const;

private:
    std::shared_ptr<Animation> runAnimation;
    std::shared_ptr<Animation> flyAnimation;
    std::shared_ptr<Animation> fallAnimation;

    const glm::vec2 groundPosition = { -400.5f, -265.5f };
    const float maxHeight = 250.0f;
    const float speed = 7.5f;
};

#endif
