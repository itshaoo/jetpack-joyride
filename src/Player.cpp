#include "Player.hpp"

Player::Player() {
    // 建立並初始化地面跑動動畫
    std::vector<std::string> runPaths = {
        RESOURCE_DIR"/Image/barry/barry0.png",
        RESOURCE_DIR"/Image/barry/barry1.png"
    };
    runAnimation = std::make_shared<Animation>(runPaths, /*z=*/5.0f);
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
    flyAnimation = std::make_shared<Animation>(flyPaths, /*z=*/5.0f);
    flyAnimation->SetPosition(groundPosition);
    flyAnimation->SetLooping(true);
    flyAnimation->SetInterval(100);
    flyAnimation->Play();
    flyAnimation->SetVisible(false); // 起始隱藏

    // 建立並初始化空中掉落動畫
    std::vector<std::string> fallPaths = {
        RESOURCE_DIR"/Image/barry/barryfalling.png"
    };
    fallAnimation = std::make_shared<Animation>(fallPaths, /*z=*/5.0f);
    fallAnimation->SetPosition(groundPosition);
    fallAnimation->SetLooping(true);
    fallAnimation->SetInterval(100);
    fallAnimation->Play();
    fallAnimation->SetVisible(false); // 起始隱藏

    // 初始化步伐音效
    stepSound = std::make_shared<Util::BGM>(RESOURCE_DIR "/Sounds/foot_step.wav");

    // 初始化重力套裝
    gravitySuit = std::make_shared<GravitySuit>();
    gravitySuit->GetRunAnimation()->SetVisible(false);
    gravitySuit->GetFlyAnimation()->SetVisible(false);
    gravitySuit->GetFallAnimation()->SetVisible(false);
    gravitySuit->GetTopAnimation()->SetVisible(false);
    gravitySuit->GetRunAnimation()->SetPosition(groundPosition);
    gravitySuit->GetFlyAnimation()->SetPosition(groundPosition);
    gravitySuit->GetFallAnimation()->SetPosition(groundPosition);
    gravitySuit->GetTopAnimation()->SetPosition(groundPosition);

    // 新增：確保 TopAnimation 會播放且循環
    gravitySuit->GetTopAnimation()->SetLooping(true);
    gravitySuit->GetTopAnimation()->SetInterval(100); // 或你想要的間隔
    gravitySuit->GetTopAnimation()->Play();

    // 初始化 Lil Stomper
    lilStomper = std::make_shared<LilStomper>();
    lilStomper->GetRunAnimation()->SetVisible(false);
    lilStomper->GetFlyAnimation()->SetVisible(false);
    lilStomper->GetFallAnimation()->SetVisible(false);
    lilStomper->GetJumpAnimation()->SetVisible(false);
    lilStomper->GetRunAnimation()->SetPosition(groundPosition);
    lilStomper->GetFlyAnimation()->SetPosition(groundPosition);
    lilStomper->GetFallAnimation()->SetPosition(groundPosition);
}

void Player::AddToRenderer(Util::Renderer &renderer) {
    // 將所有動畫加入 Renderer，由 Renderer 負責 Draw()
    renderer.AddChild(runAnimation);
    renderer.AddChild(flyAnimation);
    renderer.AddChild(fallAnimation);
    renderer.AddChild(gravitySuit->GetRunAnimation());
    renderer.AddChild(gravitySuit->GetFlyAnimation());
    renderer.AddChild(gravitySuit->GetFallAnimation());
    renderer.AddChild(gravitySuit->GetTopAnimation());
    renderer.AddChild(lilStomper->GetRunAnimation());
    renderer.AddChild(lilStomper->GetFlyAnimation());
    renderer.AddChild(lilStomper->GetFallAnimation());
    renderer.AddChild(lilStomper->GetJumpAnimation());
}

void Player::Update() {
    bool isSpacePressed = Util::Input::IsKeyPressed(Util::Keycode::SPACE);
    bool spaceJustPressed = isSpacePressed && !lastSpacePressed;

    std::shared_ptr<Animation> curRun, curFly, curFall, cur;
    if (hasGravitySuit) {
        curRun  = gravitySuit->GetRunAnimation();
        curFly  = gravitySuit->GetFlyAnimation();
        curFall = gravitySuit->GetFallAnimation();
        cur  = gravitySuit->GetTopAnimation();
    } else if (hasLilStomper) {
        curRun  = lilStomper->GetRunAnimation();
        curFly  = lilStomper->GetFlyAnimation();
        curFall = lilStomper->GetFallAnimation();
        cur = lilStomper->GetJumpAnimation();
    } else {
        curRun  = runAnimation;
        curFly  = flyAnimation;
        curFall = fallAnimation;
        cur  = nullptr;
    }

    // Hide all animations
    runAnimation->SetVisible(false);
    flyAnimation->SetVisible(false);
    fallAnimation->SetVisible(false);
    gravitySuit->GetRunAnimation()->SetVisible(false);
    gravitySuit->GetFlyAnimation()->SetVisible(false);
    gravitySuit->GetFallAnimation()->SetVisible(false);
    gravitySuit->GetTopAnimation()->SetVisible(false);
    lilStomper->GetRunAnimation()->SetVisible(false);
    lilStomper->GetFlyAnimation()->SetVisible(false);
    lilStomper->GetFallAnimation()->SetVisible(false);
    lilStomper->GetJumpAnimation()->SetVisible(false);

    glm::vec2 pos = curRun->GetPosition();

    if (hasGravitySuit) {
    switch (state) {
        case PlayerState::Running:
            if (spaceJustPressed) {
                state = PlayerState::FlyingUp;
                curFly->SetVisible(true);
            } else {
                curRun->SetVisible(true);
                int frame = gravitySuit->GetRunAnimation()->GetCurrentFrameIndex();
                if (frame != lastFrameIndex) {
                    stepSound->Play(0);
                    lastFrameIndex = frame;
                }
            }
            break;

        case PlayerState::FlyingUp:
            if (spaceJustPressed) {
                state = PlayerState::FallingDown;
                curFall->SetVisible(true);
            } else {
                curFly->SetVisible(true);
                if (pos.y < maxHeight) {
                    pos.y += speed;
                } else {
                    pos.y = maxHeight;
                }

            }
            break;

        case PlayerState::FallingDown:
            if (spaceJustPressed) {
                state = PlayerState::FlyingUp;
                curFly->SetVisible(true);
            } else {
                if (pos.y > groundPosition.y) {
                    pos.y -= speed;
                    curFall->SetVisible(true);
                } else {
                    pos.y = groundPosition.y;
                    state = PlayerState::Running;
                    curRun->SetVisible(true);
                    int frame = curRun->GetCurrentFrameIndex();
                    if (frame != lastFrameIndex) {
                        stepSound->Play(0);
                        lastFrameIndex = frame;
                    }
                }
            }
            break;

        case PlayerState::AtTop:
            if (spaceJustPressed) {
                state = PlayerState::FlyingUp;
                curFly->SetVisible(true);
            } else {
                cur->SetVisible(true);
                int frame = cur->GetCurrentFrameIndex();
                if (frame != lastFrameIndex) {
                    stepSound->Play(0);
                    lastFrameIndex = frame;
                }
            }
            break;
    }

    // 更新 Gravity Suit 動畫的位置（要設 3 個）
    gravitySuit->GetRunAnimation()->SetPosition(pos);
    gravitySuit->GetFlyAnimation()->SetPosition(pos);
    gravitySuit->GetFallAnimation()->SetPosition(pos);
    gravitySuit->GetTopAnimation()->SetPosition(pos);

    } else if (hasLilStomper) {
        // Lil Stomper control logic
        switch (state) {
        case PlayerState::Running:
            curRun->SetVisible(true);
            if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
                cur->SetVisible(true);
                cur->SetLooping(false);
                cur->Play();
                if (pos.y < 0.0f) {
                    pos.y += speed;
                } else {
                    pos.y -= speed + 5;
                }
            }
            else if (isSpacePressed) {
                state = PlayerState::FlyingUp;
                // 只在剛進入 FlyingUp 狀態時設置為不循環並播放一次
                curFly->SetLooping(false);
                curFly->Play();

            }
            break;

        case PlayerState::FlyingUp:
            if (!isSpacePressed) {
                state = PlayerState::FallingDown;
            }else {
                curFly->SetVisible(true);
                if (pos.y < maxHeight) {
                    pos.y += speed;
                } else {
                    pos.y = maxHeight;
                }
            }
            break;

        case PlayerState::FallingDown:
            if (spaceJustPressed) {
                state = PlayerState::FlyingUp;
                curFly->SetVisible(true);
            } else {
                if (pos.y > groundPosition.y) {
                    pos.y -= speed + 5;
                    curFall->SetVisible(true);
                } else {
                    pos.y = groundPosition.y;
                    state = PlayerState::Running;
                    curRun->SetVisible(true);
                }
            }
            break;
        }
    } else {
        // ===== 沒有 Gravity Suit 的舊邏輯 =====
        if (isSpacePressed) {
        // 按下 SPACE 時，角色往上飛
            if (pos.y < maxHeight) {
                pos.y += speed;
            }
            curRun->SetVisible(false);
            curFly->SetVisible(true);
            curFall->SetVisible(false);
        } else {
            // 沒有按 SPACE 時，角色會下落
            if (pos.y > groundPosition.y) {
                pos.y -= speed;
                curRun->SetVisible(false);
                curFly->SetVisible(false);
                curFall->SetVisible(true);
            }

            if (pos.y <= groundPosition.y) {
                pos.y = groundPosition.y;
                curRun->SetVisible(true);
                curFly->SetVisible(false);
                curFall->SetVisible(false);

                // 播放步伐音效
                int currentFrameIndex = curRun->GetCurrentFrameIndex();
                if (currentFrameIndex != lastFrameIndex) {
                    stepSound->Play(0);
                    lastFrameIndex = currentFrameIndex;
                }
            }
        }
    }

    // 更新動畫位置
    curRun->SetPosition(pos);
    curFly->SetPosition(pos);
    curFall->SetPosition(pos);
    if (cur) cur->SetPosition(pos);

    // 累積移動距離
    m_Distance += speed * (Util::Time::GetDeltaTimeMs() / 500.0f);

    // 更新按鍵狀態紀錄
    lastSpacePressed = isSpacePressed;
}

glm::vec2 Player::GetPosition() const {
    if (hasGravitySuit) {
        return gravitySuit->GetRunAnimation()->GetPosition();
    } else if (hasLilStomper) {
        return lilStomper->GetRunAnimation()->GetPosition();
    } else {
        return runAnimation->GetPosition();
    }
}

void Player::EnableGravitySuit() {
    hasGravitySuit = true;
    hasLilStomper = false;

    // 同步位置，將重力套裝的所有動畫位置設為當前 Barry 的位置
    glm::vec2 currentPos = runAnimation->GetPosition();
    gravitySuit->GetRunAnimation()->SetPosition(currentPos);
    gravitySuit->GetFlyAnimation()->SetPosition(currentPos);
    gravitySuit->GetFallAnimation()->SetPosition(currentPos);
    gravitySuit->GetTopAnimation()->SetPosition(currentPos);
}

void Player::EnableLilStomper() {
    hasLilStomper = true;
    hasGravitySuit = false;
    groundPosition.y = -220.5f; // Reset ground position for Lil Stomper

    glm::vec2 currentPos = runAnimation->GetPosition();
    lilStomper->GetRunAnimation()->SetPosition(currentPos);
    lilStomper->GetFlyAnimation()->SetPosition(currentPos);
    lilStomper->GetFallAnimation()->SetPosition(currentPos);
    lilStomper->GetJumpAnimation()->SetPosition(currentPos);
}

void Player::DisableGravitySuit() {
    hasGravitySuit = false;
}

void Player::DisableLilStomper() {
    hasLilStomper = false;
    groundPosition.y = -265.5f; // Reset ground position for normal state
}