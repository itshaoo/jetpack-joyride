#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Player.hpp"
#include "missile.hpp"
#include "Camera.hpp"
#include "Util/BGM.hpp"
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
    m_Root.AddChild(m_DistanceText); 

    m_Player = std::make_shared<Player>();
    m_Player->AddToRenderer(m_Root);

    m_ZapperManager.SetRenderer(&m_Root);
    m_CoinManager.SetRenderer(&m_Root);

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
    }

    if (m_BackgroundStarted) {
        m_Background.Update();
        m_Player->Update();

        float playerDistance = m_Player->GetDistance(); 
        m_DistanceText->UpdateDistance(playerDistance); 

        Camera::GetInstance().Update(0.016f);

        m_ZapperManager.Update(0.016f);

        m_CoinManager.Update(0.016f);

        // 更新火箭生成邏輯，傳遞 Barry 的位置
        Missile::UpdateMissiles(m_MissileSpawnInterval, m_Missiles, m_Root, m_Player->GetPosition());
        Equipment::UpdateEquipments(EquipmentspawnInterval, equipments, m_Root, backgroundSpeed);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    std::cout << "Distance: " << m_Player->GetDistance() << std::endl;

    m_Root.Update();
}

void App::End() {
    LOG_TRACE("End");
}