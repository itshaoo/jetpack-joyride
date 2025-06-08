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
    m_LastPlayerWalkDist   = 0.0f;
    m_CurrentWalkDistance  = 0.0f;
    m_Completed            = false;
}

void Level4::Update() {
    if (m_Completed) return;

    // 1) 从 Player 拿到“总步行距离”
    float totalWalk = m_App->GetPlayer()->GetWalkDistance();

    // 2) 判断玩家是否处于“贴地面”或“贴天花板”状态
    bool onGroundOrCeiling =
        m_App->GetPlayer()->IsOnGround()
     || m_App->GetPlayer()->IsOnCeiling();

    // 3) 只有在“贴地/贴顶”时才累计步行距离的增量
    if (onGroundOrCeiling) {
        float delta = totalWalk - m_LastPlayerWalkDist;
        if (delta > 0.0f) {
            m_CurrentWalkDistance += delta;
        }
    }

    // 4) 更新上一帧的总步行里程
    m_LastPlayerWalkDist = totalWalk;

    // 5) 判断是否达标
    if (m_CurrentWalkDistance >= m_TargetDistance) {
        m_CurrentWalkDistance = m_TargetDistance;
        m_Completed = true;
    }
}

void Level4::Render() {}