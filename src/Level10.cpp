#include "Level10.hpp"
#include "App.hpp"
#include "Util/Text.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"
#include "config.hpp"

Level10::Level10(App* app)
  : m_App(app)
{ }

void Level10::Start() {
    // 1) 啟動遊戲主迴圈──初始化背景、玩家、CollisionManager 等
    m_App->Start();

    // 2) 重置里程與狀態
    m_StartDistance = -1.0f;
    m_CurrentVehicleDistance = 0.0f;
    m_Completed = false;
}

void Level10::Update() {
    if (m_Completed) return;

    // 3) 檢查玩家是否「穿著 Vehicle」
    bool wearing = m_App->GetPlayer()->IsWearingVehicle();

    // 4) 如果此刻沒有穿著 Vehicle，就重置起始里程
    if (!wearing) {
        m_StartDistance = -1.0f;
        m_CurrentVehicleDistance = 0.0f;
        return;
    }

    // 5) 穿著 Vehicle 且 m_StartDistance<0，表示「剛剛穿上」：
    //    以當前 Player::GetDistance() 作為基準起點
    float playerTotalDist = m_App->GetDistance();
    if (wearing && m_StartDistance < 0.0f) {
        m_StartDistance = playerTotalDist;
    }

    // 6) 玩家保持穿著 Vehicle，且 m_StartDistance >= 0，就計算累積距離
    if (m_StartDistance >= 0.0f) {
        m_CurrentVehicleDistance = playerTotalDist - m_StartDistance;
        if (m_CurrentVehicleDistance < 0.0f) {
            // 保險：若 somehow 總里程倒退，還是至少顯示 0
            m_CurrentVehicleDistance = 0.0f;
        }
        if (m_CurrentVehicleDistance >= kTargetDistance) {
            // 到達 700 米
            m_CurrentVehicleDistance = kTargetDistance;
            m_Completed = true;
        }
    }
}

void Level10::Render() {}