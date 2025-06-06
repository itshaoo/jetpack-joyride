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
#include "CollisionManager.hpp"
#include "CoinCounter.hpp"
#include "DistanceCounter.hpp"
#include "Util/BGM.hpp"
#include "ILevel.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level4.hpp"
#include "Level6.hpp"
#include "Level7.hpp"
#include "Level8.hpp"
#include "LevelSelect.hpp"
#include "MissionDescription.hpp"
#include "PauseMenu.hpp"
#include "BestDistance.hpp"
#include <vector>
#include <memory>
#include <fstream>

class App {
public:
    enum class State {
        LEVEL_SELECT,
        MISSION_DESCRIPTION,
        LEVEL1,
        LEVEL2,
        LEVEL4,
        LEVEL6,
        LEVEL7,
        LEVEL8,
        START,
        UPDATE,
        PAUSED,
        GAMEOVER,
        END,
    };

    enum class SpawnPhase { ZAPPER, COIN, EQUIP};

    App();               // 新增建構子
    ~App() = default;

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void Render();
    void End();

    void ResetGame();

    CollisionManager* GetCollisionManager();
    PauseMenu* GetPauseMenu();

    float GetDistance() const { return m_Distance; }

    Background* GetBackground() { return &m_Background; }
    Player*     GetPlayer()     { return m_Player.get();  }

    // 返回第四关当前已触碰红灯数量（0~10）
    int GetLevel4RedCount() const;
    // 返回第四关的目标步行距离（500）与当前距离
    float GetLevel4Distance() const;

    // 返回第八关当前已收集装备数量（0~2）
    int GetLevel8EquipCount() const;
private:
    State m_CurrentState = State::LEVEL_SELECT;
    State m_PreviousState = State::UPDATE;
    std::unique_ptr<ILevel> m_LevelSelect;
    std::unique_ptr<ILevel> m_MissionDesc;

    ILevel* m_CurrentLevel = nullptr;
    std::unique_ptr<Level1> m_Level1;
    std::unique_ptr<Level2> m_Level2;
    std::unique_ptr<Level4> m_Level4;
    std::unique_ptr<Level6> m_Level6;
    std::unique_ptr<Level7> m_Level7;
    std::unique_ptr<Level8> m_Level8;
    int m_CurrentLevelNumber = 1;

    Logo m_Logo;
    Background m_Background;
    bool m_BackgroundStarted = false;

    Util::Renderer m_Root;

    bool m_isSpacePressed = false;
    std::shared_ptr<Player> m_Player;

    // 背景滾動速度
    float backgroundSpeed = 4.0f;

    // 管理障礙物與硬幣
    ZapperManager m_ZapperManager;
    CoinManager   m_CoinManager;

    // 火箭
    std::vector<std::shared_ptr<Missile>> m_Missiles;
    float m_MissileSpawnTimer = 0.0f;
    const float m_MissileSpawnInterval = 5000.0f;

    // 道具
    std::vector<std::shared_ptr<Equipment>> equipments;
    float EquipmentspawnInterval = 2000.0f;

    std::unique_ptr<CollisionManager> m_CollisionMgr;

    std::shared_ptr<CoinCounter> m_CoinCounter;

    float m_Distance = 0.0f; // 距离
    std::shared_ptr<DistanceCounter> m_DistanceCounter;

    // 暫停
    bool m_paused = false;
    std::unique_ptr<PauseMenu> m_PauseMenu;

    std::shared_ptr<BestDistance> m_BestDistance;

    SpawnPhase m_SpawnPhase = SpawnPhase::ZAPPER;
    int m_ZapperWavesLeft = 3;
    int m_CoinWavesLeft = 2;
    bool      m_HasSpawnedCurrentWave;
    bool m_EquipSpawnedOnce = false;

    void GameUpdate();   // 每帧更新背景、玩家、障碍、金币、碰撞……（原 UPDATE 分支内容）
    void GameRender();   // 每帧绘制背景、玩家、障碍、金币等，及暂停覆盖（原 Render 中 default 分支内容）
};

#endif