#include "App.hpp"
#include "LevelSelect.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "config.hpp"
#include <iostream>

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

void App::Start() {
    LOG_TRACE("Start");

    ResetGame();

    // 初始化背景音樂
    Util::BGM bgm(RESOURCE_DIR  "/Sounds/background_music.wav");
    bgm.SetVolume(50);
    bgm.Play(-1);

    // 一開始呼叫 Render，畫出 Logo
    Render();

    // 初始化遊戲物件
    m_DistanceCounter = std::make_shared<DistanceCounter>();
    m_BestDistance = std::make_shared<BestDistance>();
    m_Player = std::make_shared<Player>();
    m_Player->AddToRenderer(m_Root);
    m_CollisionMgr = std::make_unique<CollisionManager>(
        m_Player.get(),
        &m_ZapperManager,
        &m_CoinManager,
        m_Missiles,
        equipments,
        &m_Root
    );
    m_CoinCounter = std::make_shared<CoinCounter>();

    // 傳遞背景速度給背景物件
    m_Background.SetBackgroundSpeed(backgroundSpeed);

    m_CurrentState = State::UPDATE;
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
    } else if (m_CurrentState == State::LEVEL7) {
        GameRender();
        m_Level7->Render();
    } else if (m_CurrentState == State::LEVEL8) {
        GameRender();
        m_Level8->Render();
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
        m_PauseMenu->Render();
    }
}

void App::GameUpdate() {
    // 計算距離達到每 1000 米時增加背景速度
    int distanceThreshold = 1000;  // 每 1000 米增加背景速度
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
                if (!m_EquipSpawnedOnce) {
                    // 第一次进入 EQUIP 阶段，调用一次 spawn
                    Equipment::UpdateEquipments(
                        EquipmentspawnInterval,
                        equipments,
                        m_Root,
                        backgroundSpeed
                    );
                    // 只要容器里 push 过一个，就认为 spawn 过了
                    if (!equipments.empty()) {
                        m_EquipSpawnedOnce = true;
                    }
                } else {
                    // 之后只更新位置并移除越界，不再 spawn
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
}

void App::Update() {
    // —— 1) 统一处理按 P 进入 Pause ——
    if (m_CurrentLevel && Util::Input::IsKeyUp(Util::Keycode::P))
    {
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
            }
        } else if (md->IsReturnChosen()) {
            // 点了 Back
            static_cast<LevelSelect*>(m_LevelSelect.get())->Start();
            m_CurrentState = State::LEVEL_SELECT;
        }
    } else if (m_CurrentState == State::LEVEL1) {
        GameUpdate();
        m_Level1->Update();
    } else if (m_CurrentState == State::LEVEL2) {
        GameUpdate();
        m_Level2->Update();
    } else if (m_CurrentState == State::LEVEL7) {
        GameUpdate();
        m_Level7->Update();
    } else if (m_CurrentState == State::LEVEL8) {
        GameUpdate();
        m_Level8->Update();
    } else if (m_CurrentState == State::UPDATE) {
        // 正常游戏模式
        GameUpdate();
    } else if (m_CurrentState == State::PAUSED) {
        // 暂停界面
        m_PauseMenu->Update();
        if (m_PauseMenu->IsResumeChosen()) {
            // Resume 回去之前的状态
            m_CurrentState = m_PreviousState;
        } else if (m_PauseMenu->IsRetryChosen()) {
            m_Logo       = Logo();
            m_Background = Background();

            ResetGame();

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
            }
        } else if (m_PauseMenu->IsQuitChosen()) {
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

            ResetGame();

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

void App::End() {
    LOG_TRACE("End");
}