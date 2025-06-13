#include "Level4.hpp"
#include "App.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"

Level4::Level4(App* app)
  : m_App(app)
{ }

void Level4::Start() {
    // 1) 启动游戏主循环（初始化背景、玩家、碰撞管理等）
    m_App->Start();

    // 2) 重置进度
    m_LastDisplayDistance  = m_App->GetDistance();
    m_CurrentWalkDistance  = 0.0f;
    m_Completed            = false;
}

void Level4::Update() {
    if (m_Completed) return;

    // 1) 从 Player 拿到“总步行距离”
    float displayDist = m_App->GetDistance();

    // 2) 判断玩家是否处于“贴地面”或“贴天花板”状态
    bool onGroundOrCeiling =
        m_App->GetPlayer()->IsOnGround()
     || m_App->GetPlayer()->IsOnCeiling();

    {
        float delta = displayDist - m_LastDisplayDistance;
        if (onGroundOrCeiling && delta > 0.0f) {
            m_CurrentWalkDistance += delta;
        }
        // 不論是否 onGround，都要更新上一幀顯示距離
        m_LastDisplayDistance = displayDist;
    }

    // 5) 判断是否达标
    if (m_CurrentWalkDistance >= m_TargetDistance) {
        m_CurrentWalkDistance = m_TargetDistance;
        m_Completed = true;
    }
}

void Level4::Render() {}