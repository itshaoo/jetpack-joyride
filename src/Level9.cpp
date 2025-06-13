#include "Level9.hpp"
#include "App.hpp"
#include "Util/Text.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"
#include "config.hpp"

Level9::Level9(App* app)
  : m_App(app)
{ }

void Level9::Start() {
    m_App->Start();

    m_LastDisplayDistance  = m_App->GetDistance();
    m_TotalCeilingDistance = 0.0f;
    m_Completed            = false;
}

void Level9::Update() {
    if (m_Completed) return;

    // 1) 取得目前 HUD 顯示的總距離
    float displayDist = m_App->GetDistance();
    bool onCeiling    = m_App->GetPlayer()->IsOnCeiling();

    // 2) 計算本幀距離增量（可能已被背景速度放大）
    float delta = displayDist - m_LastDisplayDistance;

    // 3) 只在貼著天花板時，才累加增量
    if (onCeiling && delta > 0.0f) {
        m_TotalCeilingDistance += delta;
        if (m_TotalCeilingDistance >= kTargetDistance) {
            m_TotalCeilingDistance = kTargetDistance;
            m_Completed = true;
        }
    }

    // 4) 不論是否在天花板，都要更新基準距離
    m_LastDisplayDistance = displayDist;
}

void Level9::Render() {}