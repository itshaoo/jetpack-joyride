#include "Player.hpp"

Player::Player() {
    // 建立並初始化地面跑動動畫
    std::vector<std::string> runPaths = {
        RESOURCE_DIR"/Image/barry/barry0.png",
        RESOURCE_DIR"/Image/barry/barry1.png"
    };
    runAnimation = std::make_shared<Animation>(runPaths);
    runAnimation->SetPosition(groundPosition);
    runAnimation->SetLooping(true);
    runAnimation->SetInterval(100);
    runAnimation->Play();

    // 建立並初始化空中飛行動畫
    std::vector<std::string> flyPaths = {
        RESOURCE_DIR"/Image/barry/barryflying0.png",
        RESOURCE_DIR"/Image/barry/barryflying1.png",
        RESOURCE_DIR"/Image/barry/barryflying2.png"
    };
    flyAnimation = std::make_shared<Animation>(flyPaths);
    flyAnimation->SetPosition(groundPosition);
    flyAnimation->SetLooping(true);
    flyAnimation->SetInterval(100);
    flyAnimation->Play();
    flyAnimation->SetVisible(false); // 起始隱藏

    // 建立並初始化空中掉落動畫
    std::vector<std::string> fallPaths = {
        RESOURCE_DIR"/Image/barry/barryfalling.png"
    };
    fallAnimation = std::make_shared<Animation>(fallPaths);
    fallAnimation->SetPosition(groundPosition);
    fallAnimation->SetLooping(true);
    fallAnimation->SetInterval(100);
    fallAnimation->Play();
    fallAnimation->SetVisible(false); // 起始隱藏
}

void Player::AddToRenderer(Util::Renderer &renderer) {
    // 將所有動畫加入 Renderer，由 Renderer 負責 Draw() 與更新順序
    renderer.AddChild(runAnimation);
    renderer.AddChild(flyAnimation);
    renderer.AddChild(fallAnimation);
}

void Player::Update() {
    // 根據 SPACE 鍵的狀態來更新 Barry 的垂直位置與動畫切換
    bool isSpacePressed = Util::Input::IsKeyPressed(Util::Keycode::SPACE);
    glm::vec2 pos = runAnimation->GetPosition();

    if (isSpacePressed) {
        // 按下 SPACE 時 Barry 往上飛，但上限不超過 maxHeight
        if (pos.y < maxHeight) {
            pos.y += speed;
        }
        // 切換為飛行動畫
        runAnimation->SetVisible(false);
        flyAnimation->SetVisible(true);
        fallAnimation->SetVisible(false);
    } else {
        // 沒按 SPACE 時 Barry 下降，直到回到地面
        if (pos.y > groundPosition.y) {
            pos.y -= speed;
            // 下降過程中顯示掉落動畫
            runAnimation->SetVisible(false);
            flyAnimation->SetVisible(false);
            fallAnimation->SetVisible(true);
        }
        if (pos.y <= groundPosition.y) {
            pos.y = groundPosition.y;
            // 到地面後切換回跑動動畫
            runAnimation->SetVisible(true);
            flyAnimation->SetVisible(false);
            fallAnimation->SetVisible(false);
        }
    }
    // 更新所有動畫的共同位置
    runAnimation->SetPosition(pos);
    flyAnimation->SetPosition(pos);
    fallAnimation->SetPosition(pos);
}

glm::vec2 Player::GetPosition() const {
    return runAnimation->GetPosition(); 
}
