#include "App.hpp"
#include "LevelSelect.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "config.hpp"
#include <iostream>
#include <SDL_mixer.h>

App::App()
  : m_ZapperManager(&m_Root, backgroundSpeed),
    m_CoinManager  (&m_Root, backgroundSpeed)
{
    // 按鈕大小與間距
    float btnSize = 100.0f;
    float spacing = 20.0f;

    // 建立選單
    m_LevelSelect = std::make_unique<LevelSelect>(btnSize, spacing);
}

void App::InitGame() {
    // 1) 完全重置游戏状态
    ResetGame();

    m_CoinCounter = std::make_shared<CoinCounter>();
    m_DistanceCounter = std::make_shared<DistanceCounter>();
    m_BestDistance    = std::make_shared<BestDistance>();

    m_Player          = std::make_shared<Player>();
    m_Player->AddToRenderer(m_Root);

    m_CollisionMgr = std::make_unique<CollisionManager>(
        m_Player.get(),
        &m_ZapperManager,
        &m_CoinManager,
        m_Missiles,
        equipments,
        &m_Root
    );

    // 4) 启动背景滚动
    m_Background.SetBackgroundSpeed(backgroundSpeed);
    m_BackgroundStarted = false;
    // Logo 会在第一帧后自动滚出
}

void App::Render() {
    if (m_CurrentState == State::LEVEL_SELECT) {
        m_LevelSelect->Render();
        return;
    } else if (m_CurrentState == State::MISSION_DESCRIPTION) {
        m_MissionDesc->Render();
        return;
    } else if (m_CurrentState == State::LEVEL1) {
        GameRender();
        m_Level1->Render();
    } else if (m_CurrentState == State::LEVEL2) {
        GameRender();
        m_Level2->Render();
    } else if (m_CurrentState == State::LEVEL3) {
        GameRender();
        m_Level3->Render();
    } else if (m_CurrentState == State::LEVEL4) {
        GameRender();
        m_Level4->Render();
    } else if (m_CurrentState == State::LEVEL5) {
        GameRender();
        m_Level5->Render();
    } else if (m_CurrentState == State::LEVEL6) {
        GameRender();
        m_Level6->Render();
    } else if (m_CurrentState == State::LEVEL7) {
        GameRender();
        m_Level7->Render();
    } else if (m_CurrentState == State::LEVEL8) {
        GameRender();
        m_Level8->Render();
    } else if (m_CurrentState == State::LEVEL9) {
        GameRender();
        m_Level9->Render();
    } else if (m_CurrentState == State::LEVEL10) {
        GameRender();
        m_Level10->Render();
    } else if (m_CurrentState == State::GAMEOVER) {
        // 1) 畫 total.png 全螢幕
        Util::Image totalImg(RESOURCE_DIR "/Image/GameOver/total.png");
        Util::Transform t;
        t.scale = {
            float(WINDOW_WIDTH)  / totalImg.GetSize().x,
            float(WINDOW_HEIGHT) / totalImg.GetSize().y
        };
        totalImg.Draw(Util::ConvertToUniformBufferData(t, totalImg.GetSize(), 0.0f));
        // 2) 繪製 Retry 按鈕 (自行調整路徑/大小/位置)
        static Util::Image retryN(RESOURCE_DIR "/Image/Pause/retry.png");
        static Util::Image retryP(RESOURCE_DIR "/Image/Pause/retry2.png");
        glm::vec2 btnPos  = { 483.0f, -260.0f };
        glm::vec2 btnSize = { 240.0f,  96.0f };
        bool down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        glm::vec2 mp = Util::Input::GetCursorPosition();
        bool over = mp.x>=btnPos.x && mp.x<=btnPos.x+btnSize.x
                    && mp.y>=btnPos.y && mp.y<=btnPos.y+btnSize.y;
        Util::Transform bt;
        bt.translation = btnPos;
        bt.scale = { btnSize.x/retryN.GetSize().x,
                        btnSize.y/retryN.GetSize().y };
        (down && over ? retryP : retryN).Draw(Util::ConvertToUniformBufferData(bt, retryN.GetSize(), 1.0f));

        {
            static Util::Image quitN(RESOURCE_DIR "/Image/Pause/quit.png");
            static Util::Image quitP(RESOURCE_DIR "/Image/Pause/quit2.png");
            // 假設將 Quit 放在 Retry 左邊
            glm::vec2 quitPos  = { -490.0f, -260.0f };
            glm::vec2 btnSize  = { 240.0f, 96.0f };
            glm::vec2 mp       = Util::Input::GetCursorPosition();
            bool down          = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
            bool overQuit      = mp.x>=quitPos.x && mp.x<=quitPos.x+btnSize.x
                              && mp.y>=quitPos.y && mp.y<=quitPos.y+btnSize.y;
            Util::Transform qt;
            qt.translation = quitPos;
            qt.scale       = { btnSize.x/quitN.GetSize().x,
                               btnSize.y/quitN.GetSize().y };
            bool isPressedQuit = down && overQuit;
            (isPressedQuit ? quitP : quitN)
              .Draw(Util::ConvertToUniformBufferData(qt, quitN.GetSize(), 1.0f));
        }

        {
            // 假設你想把距離放在畫面上方中間偏下一點
            float baseX      = -50.0f;
            float baseY      =  100.0f;
            float spacing    =  36.0f;  // 间距也放大一致
            float digitScale =  1.35f;   // 原来 0.9 变成 1.35

            int distance = static_cast<int>(m_Distance);
            std::string ds = std::to_string(distance);

            for (size_t i = 0; i < ds.size(); ++i) {
                char c = ds[i];
                std::string path = std::string(RESOURCE_DIR) + "/Image/Count/g" + c + ".png";
                Util::Image img(path);
                Util::Transform tr;
                tr.translation = {
                    baseX + float(i) * spacing,
                    baseY
                };
                tr.scale = { digitScale, digitScale };
                img.Draw(Util::ConvertToUniformBufferData(tr, img.GetSize(), 1.0f));
            }
        }

        // 4) 顯示 總金幣 （用金幣 HUD 圖示）
        {
            // 放在距離下面
            float baseX     = 50.0f;
            float baseY     = -155.0f;
            float spacing   = 40.0f;
            float coinScale = 2.0f;
            int coins = m_CollisionMgr->GetCoinCount();

            std::string cs = std::to_string(coins);

            // 畫每個數字
            for (size_t i = 0; i < cs.size(); ++i) {
                char c = cs[i];
                std::string path = std::string(RESOURCE_DIR) + "/Image/Count/" + c + ".png";
                Util::Image img(path);
                Util::Transform tr;
                tr.translation = { baseX + float(i) * spacing, baseY };
                tr.scale       = { coinScale, coinScale };
                img.Draw(Util::ConvertToUniformBufferData(tr, img.GetSize(), 1.0f));
            }
        }

        return;
    } else {
        GameRender();
    }

    // 如果在暫停，疊加暫停頁面
    if (m_CurrentState == State::PAUSED) {
        // 准备两行文字对象
        static std::shared_ptr<Util::Text> line1 =
            std::make_shared<Util::Text>(
                RESOURCE_DIR "/font/OpenSans_Condensed-ExtraBold.ttf",
                28,
                " ",
                Util::Color::FromName(Util::Colors::WHITE)
            );
        static std::shared_ptr<Util::Text> line2 =
            std::make_shared<Util::Text>(
                RESOURCE_DIR "/font/OpenSans_Condensed-ExtraBold.ttf",
                28,
                " ",
                Util::Color::FromName(Util::Colors::WHITE)
            );

        bool needDrawProgress = false;

        if (m_CurrentLevelNumber == 3 && m_Level3) {
            int  redCount = GetLevel3RedCount();
            line1->SetText("RED LIGHT: " + std::to_string(redCount) + "/10");
            line2->SetText(" ");
            needDrawProgress = true;
        } else if (m_CurrentLevelNumber == 4 && m_Level4) {
            float raw = GetLevel4Distance();
            int shown = std::min( static_cast<int>(raw), static_cast<int>(static_cast<Level4*>(m_Level4.get())->GetTargetDistance()) );
            line1->SetText("DISTANCE: " + std::to_string(shown) + "/500");
            line2->SetText(" ");
            needDrawProgress = true;
        } else if (m_CurrentLevelNumber == 5 && m_Level5) {
            float fd = static_cast<Level5*>(m_Level5.get())->GetCurrentFlightDistance();
            line1->SetText("DISTANCE: " + std::to_string(static_cast<int>(fd)) + "/500");
            line2->SetText(" ");
            needDrawProgress = true;
        } else if (m_CurrentLevelNumber == 6 && m_Level6) {
            int coins = static_cast<Level6*>(m_Level6.get())->GetCurrentCoinCount();
            line1->SetText("COIN: " + std::to_string(coins) + "/" + std::to_string(Level6::kTargetCoins));
            line2->SetText(" ");
            needDrawProgress = true;
        } else if (m_CurrentLevelNumber == 8 && m_Level8) {
            int  equipCount = GetLevel8EquipCount();
            line1->SetText("VEHICLE: " + std::to_string(equipCount) + "/2");
            line2->SetText(" ");
            needDrawProgress = true;
        } else if (m_CurrentLevelNumber == 9 && m_Level9) {
            float cd = static_cast<Level9*>(m_Level9.get())->GetCurrentCeilingDistance();
            line1->SetText("DISTANCE: " + std::to_string(static_cast<int>(cd)) + "/300");
            line2->SetText(" ");
            needDrawProgress = true;
        } else if (m_CurrentLevelNumber == 10 && m_Level10) {
            float vd = static_cast<Level10*>(m_Level10.get())->GetCurrentVehicleDistance();
            line1->SetText("DISTANCE: " + std::to_string(static_cast<int>(vd)) + "/700");
            line2->SetText(" ");
            needDrawProgress = true;
        }

        if (needDrawProgress) {
            // 先算出两行文字的宽高，方便居中对齐 + 向右/向下微调
            glm::vec2 size1 = line1->GetSize();
            glm::vec2 size2 = line2->GetSize();
            float lineSpacing = 10.0f;             // 行间距
            float blockWidth  = std::max(size1.x, size2.x);
            float blockHeight = size1.y + size2.y + lineSpacing;

            // 希望在居中基础上“向右 50、向下 30”
            float offsetX = 50.0f;
            float offsetY = 30.0f;

            // 画第一行
            Util::Transform t1;
            t1.translation.x = -blockWidth * 0.5f + offsetX;
            t1.translation.y =  blockHeight * 0.5f - size1.y * 0.5f - offsetY;
            line1->Draw(Util::ConvertToUniformBufferData(t1, size1, 2.0f));

            // 画第二行（紧接在第一行下面）
            Util::Transform t2;
            t2.translation.x = -blockWidth * 0.5f + offsetX;
            t2.translation.y = t1.translation.y - (size1.y * 0.5f + lineSpacing + size2.y * 0.5f);
            line2->Draw(Util::ConvertToUniformBufferData(t2, size2, 2.0f));
        }

        // 再把原来的暂停菜单（按钮、半透明蒙版等）画上去
        m_PauseMenu->Render();
    }
}

void App::GameUpdate() {
    // 計算距離達到每 1000 米時增加背景速度
    int distanceThreshold = 300;  // 每 1000 米增加背景速度
    float speedIncrement = 0.5f;  // 每次增加的背景速度

    float playerDistance = m_Player->GetDistance();
    m_Distance = playerDistance;  // 同步更新 m_Distance

    // 根據玩家距離計算增量次數
    int distanceThresholdsCrossed = static_cast<int>(playerDistance / distanceThreshold);
    // 計算背景速度增量
    backgroundSpeed = 4.0f + distanceThresholdsCrossed * speedIncrement;

    // 更新背景速度
    m_Background.SetBackgroundSpeed(backgroundSpeed);

    // Logo & 背景
    m_Logo.Update();

    if (!m_BackgroundStarted && m_Logo.IsOffScreen()) {
        m_BackgroundStarted = true;

        if (!m_BgmStarted) {
            m_BGM.SetVolume(50);
            m_BGM.Play(-1);
            m_BgmStarted = true;
        }

        m_Root.AddChild(m_CoinCounter);
        m_Root.AddChild(m_DistanceCounter);
        m_Root.AddChild(m_BestDistance);
        m_Background.NotifyLogoOffScreen();
    }

    if (m_BackgroundStarted) {
        m_Background.Update();
        m_Player->Update();

        float playerDistance = m_Player->GetDistance();
        m_DistanceCounter->SetDistance(playerDistance);

        switch (m_SpawnPhase) {
            case SpawnPhase::ZAPPER:
                // 如果还没手动 spawn 本波，就 spawn 一次
                if (!m_HasSpawnedCurrentWave) {
                    m_ZapperManager.SpawnZappers();
                    m_HasSpawnedCurrentWave = true;
                }
                // 更新现有 Zappers（移动 & 移除）
                m_ZapperManager.UpdateExisting(backgroundSpeed);
                // 全部清光，才算本波结束
                if (m_ZapperManager.GetZappers().empty()) {
                    if (--m_ZapperWavesLeft > 0) {
                        m_HasSpawnedCurrentWave = false;  // 下一波还得再 spawn
                    } else {
                        m_SpawnPhase = SpawnPhase::COIN;
                        m_HasSpawnedCurrentWave = false;
                    }
                }
                break;

            case SpawnPhase::COIN:
                if (!m_HasSpawnedCurrentWave) {
                    m_CoinManager.SpawnCoinPattern();
                    m_HasSpawnedCurrentWave = true;
                }
                m_CoinManager.UpdateExisting(backgroundSpeed);
                if (m_CoinManager.GetCoins().empty()) {
                    if (--m_CoinWavesLeft > 0) {
                        m_HasSpawnedCurrentWave = false;
                    } else {
                        m_SpawnPhase = SpawnPhase::EQUIP;
                        m_HasSpawnedCurrentWave = false;
                    }
                }
                break;

            case SpawnPhase::EQUIP:
                // 如果玩家已经穿着装备，就跳过这一阶段
                if (m_Player->IsWearingVehicle()) {
                    // 直接重置 spawnPhase 到下一波障碍
                    m_SpawnPhase            = SpawnPhase::ZAPPER;
                    m_HasSpawnedCurrentWave = false;
                    m_EquipSpawnedOnce      = false;
                    // 重设下一轮波次数
                    m_ZapperWavesLeft = 1;
                    m_CoinWavesLeft   = 3;
                    break;
                }
                // 正常的装备生成／移动逻辑
                if (!m_EquipSpawnedOnce) {
                     Equipment::UpdateEquipments(
                        EquipmentspawnInterval,
                        equipments,
                        m_Root,
                        backgroundSpeed
                     );
                     if (!equipments.empty()) {
                        m_EquipSpawnedOnce = true;
                     }
                } else {
                    for (auto it = equipments.begin(); it != equipments.end(); ) {
                        auto& eq = *it;
                        eq->Update(backgroundSpeed);
                        if (eq->IsOffScreen()) {
                            m_Root.RemoveChild(eq->GetAnimation());
                            it = equipments.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
                // 所有装备都处理完毕后，回到下一轮 ZAPPER
                if (m_EquipSpawnedOnce && equipments.empty()) {
                    m_SpawnPhase            = SpawnPhase::ZAPPER;
                    m_HasSpawnedCurrentWave = false;
                    m_EquipSpawnedOnce      = false;
                    // 重设下一轮波次计数
                    m_ZapperWavesLeft = 1;
                    m_CoinWavesLeft   = 3;
                }
                break;
        }

        Missile::UpdateMissiles(
            m_MissileSpawnInterval,
            m_Missiles,
            m_Root,
            m_Player->GetPosition()
        );

        if (m_CollisionMgr->Update()) {
            Mix_HaltChannel(-1);
            m_CurrentState = State::GAMEOVER;
        }
        m_CoinCounter->SetCount(m_CollisionMgr->GetCoinCount());
    }
    m_Root.Update();
}

void App::GameRender() {
    if (!m_BackgroundStarted) {
        m_Logo.Render();
    }

    m_Background.Render();
}

void App::ResetGame() {
    // 1) 清空 renderer 上所有 child
    m_Root = Util::Renderer();

    // 2) 清掉所有遊戲物件容器
    m_Missiles.clear();
    equipments.clear();

    // 3) 重建各個 manager，並把它們綁到新的 renderer
    m_ZapperManager = ZapperManager(&m_Root, backgroundSpeed);
    m_CoinManager   = CoinManager(&m_Root, backgroundSpeed);

    // —— 在這裡重置 spawn 階段與波次 ——
    m_SpawnPhase      = SpawnPhase::ZAPPER;
    m_ZapperWavesLeft = /* 你希望的 Zapper 波次數，例如 */ 1;
    m_CoinWavesLeft   = /* 你希望的 Coin 波次數，例如 2 */ 3;
    m_EquipSpawnedOnce = false;
    Equipment::ResetSpawnTimer();
    m_HasSpawnedCurrentWave = false;
    // 如果需要，還可以在這裡呼叫：

    // 4) 清除舊的 HUD / Counters / Player
    m_Player.reset();
    m_CoinCounter.reset();
    m_DistanceCounter.reset();
    m_BestDistance.reset();

    m_BackgroundStarted = false;

    m_Distance = 0.0f;
}

void App::Update() {
    bool isInGameplay =
       m_CurrentState == State::UPDATE
    || m_CurrentState == State::LEVEL1
    || m_CurrentState == State::LEVEL2
    || m_CurrentState == State::LEVEL3
    || m_CurrentState == State::LEVEL4
    || m_CurrentState == State::LEVEL5
    || m_CurrentState == State::LEVEL6
    || m_CurrentState == State::LEVEL7
    || m_CurrentState == State::LEVEL8
    || m_CurrentState == State::LEVEL9
    || m_CurrentState == State::LEVEL10;

    if (isInGameplay && Util::Input::IsKeyUp(Util::Keycode::P)) {
        Mix_Pause(-1);
        m_PreviousState = m_CurrentState;
        // 构造并启动暂停界面
        auto* md = static_cast<MissionDescription*>(m_MissionDesc.get());
        std::string text = md->GetMissionString();

        // 直接問目前那一關：完成了沒？
        bool levelDone = m_CurrentLevel->IsCompleted();

        m_PauseMenu = std::make_unique<PauseMenu>(
            150.0f, 40.0f,
            text, levelDone
        );
        m_PauseMenu->Start();
        m_CurrentState = State::PAUSED;
        return;
    }

    // —— 2) 用 switch 处理各状态的更新 ——
    if (m_CurrentState == State::LEVEL_SELECT) {
        // 选关菜单
        m_LevelSelect->Update();
        auto* ls = static_cast<LevelSelect*>(m_LevelSelect.get());
        if (ls->IsLevelChosen()) {
            int lvl = ls->GetChosenLevel();
            m_MissionDesc = std::make_unique<MissionDescription>(lvl);
            m_MissionDesc->Start();
            m_CurrentState = State::MISSION_DESCRIPTION;
        }
    } else if (m_CurrentState == State::MISSION_DESCRIPTION) {
        // 任务说明页
        m_MissionDesc->Update();
        auto* md = static_cast<MissionDescription*>(m_MissionDesc.get());
        if (md->IsPlayChosen()) {
            InitGame();
            m_CurrentLevelNumber = md->GetLevel();
            // 根據關卡編號實例化對應的關卡（目前只有第一關）
            if (m_CurrentLevelNumber == 1) {
                m_Level1 = std::make_unique<Level1>(this);
                m_Level1->Start();
                m_CurrentState = State::LEVEL1;
                m_CurrentLevel   = m_Level1.get();
            } else if (m_CurrentLevelNumber == 2) {
                m_Level2 = std::make_unique<Level2>(this);
                m_Level2->Start();
                m_CurrentState = State::LEVEL2;
                m_CurrentLevel   = m_Level2.get();
            } else if (m_CurrentLevelNumber == 3) {
                m_Level3 = std::make_unique<Level3>(this);
                m_Level3->Start();
                m_CurrentState = State::LEVEL3;
                m_CurrentLevel   = m_Level3.get();
            }  else if (m_CurrentLevelNumber == 4) {
                m_Level4 = std::make_unique<Level4>(this);
                m_Level4->Start();
                m_CurrentState = State::LEVEL4;
                m_CurrentLevel   = m_Level4.get();
            } else if (m_CurrentLevelNumber == 5) {
                m_Level5 = std::make_unique<Level5>(this);
                m_Level5->Start();
                m_CurrentState = State::LEVEL5;
                m_CurrentLevel   = m_Level5.get();
            }  else if (m_CurrentLevelNumber == 6) {
                m_Level6 = std::make_unique<Level6>(this);
                m_Level6->Start();
                m_CurrentState = State::LEVEL6;
                m_CurrentLevel   = m_Level6.get();
            } else if (m_CurrentLevelNumber == 7) {
                m_Level7 = std::make_unique<Level7>(this);
                m_Level7->Start();
                m_CurrentState = State::LEVEL7;
                m_CurrentLevel   = m_Level7.get();
            } else if (m_CurrentLevelNumber == 8) {
                m_Level8 = std::make_unique<Level8>(this);
                m_Level8->Start();
                m_CurrentState = State::LEVEL8;
                m_CurrentLevel   = m_Level8.get();
            }  else if (m_CurrentLevelNumber == 9) {
                m_Level9 = std::make_unique<Level9>(this);
                m_Level9->Start();
                m_CurrentState = State::LEVEL9;
                m_CurrentLevel   = m_Level9.get();
            }  else if (m_CurrentLevelNumber == 10) {
                m_Level10 = std::make_unique<Level10>(this);
                m_Level10->Start();
                m_CurrentState = State::LEVEL10;
                m_CurrentLevel   = m_Level10.get();
            }
        } else if (md->IsReturnChosen()) {
            if (m_BgmStarted) {
                m_BGM.FadeOut(0);
                m_BgmStarted = false;
            }

            static_cast<LevelSelect*>(m_LevelSelect.get())->Start();
            m_CurrentState = State::LEVEL_SELECT;
        }
    } else if (m_CurrentState == State::LEVEL1) {
        GameUpdate();
        m_Level1->Update();
    } else if (m_CurrentState == State::LEVEL2) {
        GameUpdate();
        m_Level2->Update();
    } else if (m_CurrentState == State::LEVEL3) {
        GameUpdate();
        m_Level3->Update();
    } else if (m_CurrentState == State::LEVEL4) {
        GameUpdate();
        m_Level4->Update();
    } else if (m_CurrentState == State::LEVEL5) {
        GameUpdate();
        m_Level5->Update();
    } else if (m_CurrentState == State::LEVEL6) {
        GameUpdate();
        m_Level6->Update();
    } else if (m_CurrentState == State::LEVEL7) {
        GameUpdate();
        m_Level7->Update();
    } else if (m_CurrentState == State::LEVEL8) {
        GameUpdate();
        m_Level8->Update();
    } else if (m_CurrentState == State::LEVEL9) {
        GameUpdate();
        m_Level9->Update();
    } else if (m_CurrentState == State::LEVEL10) {
        GameUpdate();
        m_Level10->Update();
    } else if (m_CurrentState == State::UPDATE) {
        // 正常游戏模式
        GameUpdate();
    } else if (m_CurrentState == State::PAUSED) {
        // 暂停界面
        m_PauseMenu->Update();
        if (m_PauseMenu->IsResumeChosen()) {
            Mix_Resume(-1);
            // Resume 回去之前的状态
            m_CurrentState = m_PreviousState;
        } else if (m_PauseMenu->IsRetryChosen()) {
            m_Logo       = Logo();
            m_Background = Background();

            InitGame();

            switch (m_CurrentLevelNumber) {
                case 1:
                    m_Level1 = std::make_unique<Level1>(this);
                    m_Level1->Start();
                    m_CurrentState = State::LEVEL1;
                    m_CurrentLevel = m_Level1.get();
                    break;
                case 2:
                    m_Level2 = std::make_unique<Level2>(this);
                    m_Level2->Start();
                    m_CurrentState = State::LEVEL2;
                    m_CurrentLevel = m_Level2.get();
                    break;
                case 3:
                    m_Level3 = std::make_unique<Level3>(this);
                    m_Level3->Start();
                    m_CurrentState = State::LEVEL3;
                    m_CurrentLevel = m_Level3.get();
                    break;
                case 4:
                    m_Level4 = std::make_unique<Level4>(this);
                    m_Level4->Start();
                    m_CurrentState = State::LEVEL4;
                    m_CurrentLevel = m_Level4.get();
                    break;
                case 5:
                    m_Level5 = std::make_unique<Level5>(this);
                    m_Level5->Start();
                    m_CurrentState = State::LEVEL5;
                    m_CurrentLevel = m_Level5.get();
                    break;
                case 6:
                    m_Level6 = std::make_unique<Level6>(this);
                    m_Level6->Start();
                    m_CurrentState = State::LEVEL6;
                    m_CurrentLevel = m_Level6.get();
                    break;
                case 7:
                    m_Level7 = std::make_unique<Level7>(this);
                    m_Level7->Start();
                    m_CurrentState = State::LEVEL7;
                    m_CurrentLevel = m_Level7.get();
                    break;
                case 8:
                    m_Level8 = std::make_unique<Level8>(this);
                    m_Level8->Start();
                    m_CurrentState = State::LEVEL8;
                    m_CurrentLevel = m_Level8.get();
                    break;
                case 9:
                    m_Level9 = std::make_unique<Level9>(this);
                    m_Level9->Start();
                    m_CurrentState = State::LEVEL9;
                    m_CurrentLevel = m_Level9.get();
                    break;
                case 10:
                    m_Level10 = std::make_unique<Level10>(this);
                    m_Level10->Start();
                    m_CurrentState = State::LEVEL10;
                    m_CurrentLevel = m_Level10.get();
                    break;
            }
        } else if (m_PauseMenu->IsQuitChosen()) {
            if (m_BgmStarted) {
                m_BGM.FadeOut(0);
                m_BgmStarted = false;
            }
            // Quit → 回任务说明
            m_Logo       = Logo();
            m_Background = Background();
            ResetGame();
            static_cast<MissionDescription*>(m_MissionDesc.get())->Start();
            m_CurrentState = State::MISSION_DESCRIPTION;
        }
    } else if (m_CurrentState == State::GAMEOVER) {
        // 在遊戲結束後，如果玩家的總距離大於最佳距離，更新 best.txt
        std::ifstream file(RESOURCE_DIR "/best.txt");
        int bestDistance = 0;
        if (file.is_open()) {
            file >> bestDistance;
        }
        file.close();

        if (m_Distance > bestDistance) {
            std::ofstream outFile(RESOURCE_DIR "/best.txt");
            outFile << m_Distance;
            outFile.close();
        }
        // 在 GameOver 畫面，處理按 Esc 直接結束 或 點 Retry
        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
            Util::Input::IfExit()) {
            m_CurrentState = State::END;
            return;
            }
        // 處理 Retry 按鈕點擊
        bool up = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
        glm::vec2 mp = Util::Input::GetCursorPosition();
        glm::vec2 btnPos  = { 483.0f, -260.0f };
        glm::vec2 btnSize = { 240.0f,  96.0f };
        bool over = mp.x>=btnPos.x && mp.x<=btnPos.x+btnSize.x
                    && mp.y>=btnPos.y && mp.y<=btnPos.y+btnSize.y;
        if (up && over) {
            // 重置整個遊戲回到選關
            m_Logo       = Logo();
            m_Background = Background();

            InitGame();

            switch (m_CurrentLevelNumber) {
                case 1:
                    m_Level1 = std::make_unique<Level1>(this);
                    m_Level1->Start();
                    m_CurrentState = State::LEVEL1;
                    m_CurrentLevel = m_Level1.get();
                    break;
                case 2:
                    m_Level2 = std::make_unique<Level2>(this);
                    m_Level2->Start();
                    m_CurrentState = State::LEVEL2;
                    m_CurrentLevel = m_Level2.get();
                    break;
                case 3:
                    m_Level3 = std::make_unique<Level3>(this);
                    m_Level3->Start();
                    m_CurrentState = State::LEVEL3;
                    m_CurrentLevel = m_Level3.get();
                    break;
                case 4:
                    m_Level4 = std::make_unique<Level4>(this);
                    m_Level4->Start();
                    m_CurrentState = State::LEVEL4;
                    m_CurrentLevel = m_Level4.get();
                    break;
                case 5:
                    m_Level5 = std::make_unique<Level5>(this);
                    m_Level5->Start();
                    m_CurrentState = State::LEVEL5;
                    m_CurrentLevel = m_Level5.get();
                    break;
                case 6:
                    m_Level6 = std::make_unique<Level6>(this);
                    m_Level6->Start();
                    m_CurrentState = State::LEVEL6;
                    m_CurrentLevel = m_Level6.get();
                    break;
                case 7:
                    m_Level7 = std::make_unique<Level7>(this);
                    m_Level7->Start();
                    m_CurrentState = State::LEVEL7;
                    m_CurrentLevel = m_Level7.get();
                    break;
                case 8:
                    m_Level8 = std::make_unique<Level8>(this);
                    m_Level8->Start();
                    m_CurrentState = State::LEVEL8;
                    m_CurrentLevel = m_Level8.get();
                    break;
                case 9:
                    m_Level9 = std::make_unique<Level9>(this);
                    m_Level9->Start();
                    m_CurrentState = State::LEVEL9;
                    m_CurrentLevel = m_Level9.get();
                    break;
                case 10:
                    m_Level10 = std::make_unique<Level10>(this);
                    m_Level10->Start();
                    m_CurrentState = State::LEVEL10;
                    m_CurrentLevel = m_Level10.get();
                    break;
            }
        }
        {
            // 處理 Quit 按鈕點擊
            bool quitUp = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
            glm::vec2 quitMp2  = Util::Input::GetCursorPosition();
            glm::vec2 quitPos2 = { -490.0f, -260.0f };
            glm::vec2 quitSize2 = { 240.0f, 96.0f };
            bool overQuit2 = quitMp2.x>=quitPos2.x && quitMp2.x<=quitPos2.x+quitSize2.x
                                      && quitMp2.y>=quitPos2.y && quitMp2.y<=quitPos2.y+quitSize2.y;
            if (quitUp && overQuit2) {
                if (m_BgmStarted) {
                    m_BGM.FadeOut(0);
                    m_BgmStarted = false;
                }
                // 先重置整個遊戲狀態
                m_Logo       = Logo();
                m_Background = Background();
                ResetGame();
                // 回到選關畫面並重置按鈕狀態
                static_cast<LevelSelect*>(m_LevelSelect.get())->Start();
                m_CurrentState = State::LEVEL_SELECT;
                return;
            }
        }

        return;
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

CollisionManager* App::GetCollisionManager() {
    return m_CollisionMgr.get();
}

PauseMenu* App::GetPauseMenu() {
    return m_PauseMenu.get();
}

int App::GetLevel3RedCount() const {
    if (m_Level3) {
        return m_Level3->GetRedTouchedCount();
    }
    return 0;
}

float App::GetLevel4Distance() const {
    return m_Player->GetWalkDistance();
}

int App::GetLevel8EquipCount() const {
    if (m_Level8) {
        return m_Level8->GetEquipCount();
    }
    return 0;
}

void App::End() {
    LOG_TRACE("End");
}