#include "Level5.hpp"
#include "App.hpp"
#include "Util/Text.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"
#include "config.hpp"

Level5::Level5(App* app)
  : m_App(app)
{ }

void Level5::Start() {
    // 1) 啟動遊戲主迴圈（初始化背景、玩家、CollisionManager …）
    m_App->Start();

    // 2) 重置所有與飛行狀態相關的成員
    m_StartDistance = -1.0f;
    m_CurrentFlightDistance = 0.0f;
    m_Completed = false;
}

void Level5::Update() {
    if (m_Completed) return;

    // --- 飛行距離邏輯 ---
    float playerDist = m_App->GetDistance();
    bool onGround = m_App->GetPlayer()->IsOnGround()
             || m_App->GetPlayer()->IsOnCeiling();

    // 1) 玩家在地面：重置 start，由下次離地時再開始計算
    if (onGround) {
        m_StartDistance = -1.0f;
        m_CurrentFlightDistance = 0.0f;
        return;
    }

    // 2) 玩家離地，而且剛剛才起飛：設定基準距離
    if (!onGround && m_StartDistance < 0.0f) {
        m_StartDistance = playerDist;
    }

    // 3) 如果已經在空中，計算目前飛行距離
    if (m_StartDistance >= 0.0f) {
        m_CurrentFlightDistance = playerDist - m_StartDistance;
        if (m_CurrentFlightDistance < 0.0f) {
            // 不可能發生，但保險起見最低設 0
            m_CurrentFlightDistance = 0.0f;
        }
        if (m_CurrentFlightDistance > kTargetFlight) {
            m_CurrentFlightDistance = kTargetFlight;
        }
        // 4) 如果飛行累積已經達標（≥500），就標記完成
        if (m_CurrentFlightDistance >= kTargetFlight) {
            m_Completed = true;
        }
    }
}

void Level5::Render() {}