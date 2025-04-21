#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Logo.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "Object.hpp"
#include "Animation.hpp"
#include "ZapperManager.hpp"
#include "CoinManager.hpp"
#include "Missile.hpp"
#include "Equipment.hpp"
#include <vector>
#include <memory>

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void Render();
    void End();

private:
    State m_CurrentState = State::START;

    Logo m_Logo;
    Background m_Background;
    bool m_BackgroundStarted = false;

    Util::Renderer m_Root;
    Util::Renderer renderer;

    bool m_isSpacePressed = false;
    std::shared_ptr<Player> m_Player;

    // 背景滾動速度
    float backgroundSpeed = 4.0f;

    // 管理障礙物與硬幣
    ZapperManager m_ZapperManager{ &m_Root, backgroundSpeed };
    CoinManager   m_CoinManager{ &m_Root, backgroundSpeed };

    // 火箭
    std::vector<std::shared_ptr<Missile>> m_Missiles;
    float m_MissileSpawnTimer = 0.0f;
    const float m_MissileSpawnInterval = 5000.0f;

    // 道具
    std::vector<std::shared_ptr<Equipment>> equipments;
    float EquipmentspawnInterval = 10000.0f;
};

#endif