#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "iostream"

void App::Start() {
    LOG_TRACE("Start");

    // 初始化背景音樂
    Util::BGM bgm(RESOURCE_DIR "/Sounds/background_music.wav");

    bgm.SetVolume(50);

    // 播放背景音樂，循環播放
    bgm.Play(-1);

    Render();

    // 初始化距离文本
    m_DistanceText = std::make_shared<DistanceText>();

    m_Player = std::make_shared<Player>();
    m_Player->AddToRenderer(m_Root);

    m_CollisionMgr = std::make_unique<CollisionManager>(
        m_Player.get(),
        &m_ZapperManager,
        &m_CoinManager,
        m_Missiles,
        equipments
    );

    m_CoinCounter = std::make_shared<CoinCounter>();

    m_CurrentState = State::UPDATE;
}

void App::Render() {
    if (!m_BackgroundStarted) {
        m_Logo.Render();
    }
    m_Background.Render();
}

void App::Update() {
    m_Logo.Update();
    m_isSpacePressed = Util::Input::IsKeyPressed(Util::Keycode::SPACE);

    if (!m_BackgroundStarted && m_Logo.IsOffScreen()) {
        m_BackgroundStarted = true;
        m_Root.AddChild(m_CoinCounter);
        m_Root.AddChild(m_DistanceText);
    }

    if (m_BackgroundStarted) {
        // 背景與角色更新
        m_Background.Update();
        m_Player->Update();

        float playerDistance = m_Player->GetDistance();
        m_DistanceText->UpdateDistance(playerDistance);

        // 更新障礙與硬幣
        m_ZapperManager.Update();
        m_CoinManager.Update();

        // 更新火箭生成邏輯
        Missile::UpdateMissiles(m_MissileSpawnInterval,
                                m_Missiles,
                                m_Root,
                                m_Player->GetPosition());
        Equipment::UpdateEquipments(EquipmentspawnInterval,
                                    equipments,
                                    m_Root,
                                    backgroundSpeed);

        if (m_CollisionMgr->Update()) {
            m_CurrentState = State::END;
        }

        m_CoinCounter->SetCount(m_CollisionMgr->GetCoinCount());
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

void App::End() {
    LOG_TRACE("End");
}