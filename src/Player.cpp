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
    stepSound = std::make_shared<Util::SFX>(RESOURCE_DIR "/Sounds/foot_step.wav");

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
    float dt = Util::Time::GetDeltaTimeMs() / 1000.0f;
    // 定義推力（上升加速度）與重力（下降加速度），可微調以獲得合適手感
    const float thrustAcc = 1200.0f;   // 當空白鍵按下時，每秒增加的上升速度
    const float gravityAcc = 900.0f;  // 當空白鍵未按時計算的下墜加速度

    bool isSpacePressed = Util::Input::IsKeyPressed(Util::Keycode::SPACE);
    bool spaceJustPressed = isSpacePressed && !lastSpacePressed;
    bool spaceJustReleased = (!isSpacePressed) && lastSpacePressed;

    float inertiaFactor = hasGravitySuit ? 0.1f : 0.5f;
    // 在輸入狀態變換時，額外衰減目前慣性以減少 overshoot：
    if (spaceJustReleased && m_VerticalVelocity > 0) {
        // 從上升到放開時，如果還帶有正的速度，先將該速度降下來
        m_VerticalVelocity *= inertiaFactor;
    }
    if (isSpacePressed && !lastSpacePressed && m_VerticalVelocity < 0) {
        // 從放開到按下時，如果速度為負，先衰減負速
        m_VerticalVelocity *= inertiaFactor;
    }

    // 若玩家穿著 Gravity Suit 並處於重力反轉模式（例如 FlyingUp 或 AtTop），則反轉加速度計算
    if (hasGravitySuit && (state == PlayerState::FlyingUp || state == PlayerState::AtTop)) {
        if (isSpacePressed) {
            // 在反轉狀態下，按下空白鍵代表「維持反向推力」，因此要往負方向（例如往天花板方向）增加速度
            m_VerticalVelocity -= thrustAcc * dt;
        } else {
            // 放開空白鍵則角色會加速回到天花板（也就是「重力」在起作用，不過這裡是正向增加，讓數值變大）
            m_VerticalVelocity += gravityAcc * dt;
        }
    } else {
        // 正常狀態下（或未穿裝備 Gravity Suit）依照原邏輯更新
        if (isSpacePressed) {
            m_VerticalVelocity += thrustAcc * dt;
        } else {
            m_VerticalVelocity -= gravityAcc * dt;
        }
    }

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
                    curFly->SetLooping(false);
                    curFly->Play();
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
                    curFall->SetLooping(false);
                    curFall->Play();
                } else {
                    curFly->SetVisible(true);
                    if (pos.y >= maxHeight) {
                        pos.y = maxHeight;
                        state = PlayerState::AtTop;
                        if (m_VerticalVelocity > 0)
                            m_VerticalVelocity = 0;
                    }
                }
                break;

            case PlayerState::FallingDown:
                if (spaceJustPressed) {
                    state = PlayerState::FlyingUp;
                    curFly->SetVisible(true);
                    curFly->SetLooping(false);
                    curFly->Play();
                } else {
                    if (pos.y > groundPosition.y) {
                        curFall->SetVisible(true);
                    }
                    if (pos.y <= groundPosition.y) {
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
                    state = PlayerState::FallingDown;
                    curFall->SetVisible(true);
                    curFall->SetLooping(false);
                    curFall->Play();
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
        const float HOLD_THRESHOLD   = 0.3f;  // 秒：短/長按分界
        const float JUMP_IMPULSE     = 500.0f;  // px/s：小跳初速度
        const float FLY_ACC          = 1200.0f; // 長按上推加速度
        const float SLOW_FALL_SPEED  = -60.0f;  // px/s：慢降固定速度(負值向下)
        const float GROUND_OFFSET    = 45.0f;   // Lil Stomper 著地偏移
        // Lil Stomper control logic
        switch (state) {
            case PlayerState::Running:
                curRun->SetVisible(true);
                if (spaceJustPressed) m_HoldTimer = 0.0f;
                if (isSpacePressed) m_HoldTimer += dt;
                if (isSpacePressed && m_HoldTimer >= HOLD_THRESHOLD) {
                    state              = PlayerState::FlyingUp;
                    m_VerticalVelocity = 0.0f;          // 清空舊慣性
                    curFly->SetVisible(true);
                    curFly->SetLooping(false);
                    curFly->Play();
                    break;
                }

                if (spaceJustReleased && m_HoldTimer < HOLD_THRESHOLD) {
                    state              = PlayerState::FlyingUp;
                    m_VerticalVelocity = JUMP_IMPULSE;
                    cur->SetVisible(true);              // Jump 動畫
                    cur->SetLooping(false);
                    cur->Play();
                }
                break;

            case PlayerState::FlyingUp:
                curFly->SetVisible(true);
                if (isSpacePressed) {
                    m_VerticalVelocity += FLY_ACC * dt;
                } else {
                    // 當不再按住且速度轉負時，自動進入下降狀態
                    if(m_VerticalVelocity < 0) {
                        state = PlayerState::FallingDown;
                        curFly->SetVisible(false);
                        curFall->SetVisible(true);
                        curFall->SetLooping(false);
                        curFall->Play();
                    }
                }
                if (pos.y >= maxHeight) {
                    pos.y = maxHeight;
                    if (isSpacePressed) {          // 仍在長按 → 慢降 Hover
                        state              = PlayerState::Hover;
                        m_VerticalVelocity = SLOW_FALL_SPEED;
                        // 動畫保持 Fly
                        } else {                       // 已放開 → 正常掉落
                            state              = PlayerState::FallingDown;
                            m_VerticalVelocity = 0.0f;
                            curFly->SetVisible(false);
                            curFall->SetVisible(true);
                            curFall->SetLooping(false);
                            curFall->Play();
                    }
                }
                break;
            case PlayerState::Hover:
                // 仍按住：維持緩慢下降
                curFly->SetVisible(true);
                m_VerticalVelocity = SLOW_FALL_SPEED;   // -60 px/s

                // 玩家放開鍵 → 正常掉落
                if (!isSpacePressed) {
                    state              = PlayerState::FallingDown;
                    curFly->SetVisible(false);
                    curFall->SetVisible(true);
                    curFall->SetLooping(false);
                    curFall->Play();
                }
                break;

            case PlayerState::FallingDown:
                curFall->SetVisible(true);
                // 中途再按一次：單跳
                if (isSpacePressed) {
                    // 將下降速度固定為緩慢下降速度（例如 -60 px/s）
                    m_VerticalVelocity = SLOW_FALL_SPEED;
                }
                // 著地判斷：當角色達到預定的地面高度時視為著地
                if (pos.y <= groundPosition.y - GROUND_OFFSET) {
                    pos.y = groundPosition.y - GROUND_OFFSET;
                    state = PlayerState::Running;
                    curRun->SetVisible(true);
                }
                break;
            default: break;
        }
        if (state != PlayerState::Running) m_HoldTimer = 0.0f;
        // 更新 Lil Stomper 相關動畫位置
        lilStomper->GetRunAnimation()->SetPosition(pos);
        lilStomper->GetFlyAnimation()->SetPosition(pos);
        lilStomper->GetFallAnimation()->SetPosition(pos);
        lilStomper->GetJumpAnimation()->SetPosition(pos);
    } else {
        // ===== 沒有 Gravity Suit 的舊邏輯 =====
        if (isSpacePressed) {
            curRun->SetVisible(false);
            curFly->SetVisible(true);
            curFall->SetVisible(false);
        } else {
            // 沒有按 SPACE 時，角色會下落
            if (pos.y > groundPosition.y) {
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

    pos.y += m_VerticalVelocity * dt;

    float effectiveGround = groundPosition.y;
    if (hasLilStomper) {
        effectiveGround = -220.5f;
    }
    if (pos.y > maxHeight) {
        pos.y = maxHeight;
        if (m_VerticalVelocity > 0)
            m_VerticalVelocity = 0;
        // 若在 Gravity Suit 的 FlyingUp 狀態且達到頂部，轉為 AtTop
        if (hasGravitySuit && state == PlayerState::FlyingUp) {
            state = PlayerState::AtTop;
        }
    }
    if (pos.y < effectiveGround) {
        pos.y = effectiveGround;
        if (m_VerticalVelocity < 0)
            m_VerticalVelocity = 0;
        // 當有特殊裝備的情況下，若下墜到地面，切換回 Running 並播放跑步音效
        if ((hasGravitySuit || hasLilStomper) && state == PlayerState::FallingDown) {
            state = PlayerState::Running;
            int frame = curRun->GetCurrentFrameIndex();
            if (frame != lastFrameIndex) {
                stepSound->Play(0);
                lastFrameIndex = frame;
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
    if (IsOnGround() || IsOnCeiling()) {
        m_WalkDistance += speed * (Util::Time::GetDeltaTimeMs() / 500.0f);
    }
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
    state = PlayerState::FallingDown;
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
    state = PlayerState::FallingDown;
    lilStomper->GetRunAnimation()->SetPosition(currentPos);
    lilStomper->GetFlyAnimation()->SetPosition(currentPos);
    lilStomper->GetFallAnimation()->SetPosition(currentPos);
    lilStomper->GetJumpAnimation()->SetPosition(currentPos);
}

void Player::DisableGravitySuit() {
    hasGravitySuit = false;
    glm::vec2 suitPos = gravitySuit->GetRunAnimation()->GetPosition();
    runAnimation->SetPosition(suitPos);
    flyAnimation->SetPosition(suitPos);
    fallAnimation->SetPosition(suitPos);
}

void Player::DisableLilStomper() {
    hasLilStomper = false;
    groundPosition.y = -265.5f; // Reset ground position for normal state
    glm::vec2 suitPos = lilStomper->GetRunAnimation()->GetPosition();
    runAnimation->SetPosition(suitPos);
    flyAnimation->SetPosition(suitPos);
    fallAnimation->SetPosition(suitPos);
}