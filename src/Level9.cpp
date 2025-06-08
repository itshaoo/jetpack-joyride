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

    m_StartDistance = -1.0f;
    m_LastCheckedDistance = 0.0f;
    m_TotalCeilingDistance = 0.0f;
    m_Completed = false;
}

void Level9::Update() {
    if (m_Completed) return;

    float playerTotalDist = m_App->GetDistance();
    bool onCeiling = m_App->GetPlayer()->IsOnCeiling();

    // 离开天花板时，只重置片段状态，不清零总累积
    if (!onCeiling) {
        m_StartDistance = -1.0f;
        return;
    }

    // 刚进入天花板：记录起始和上次检查点
    if (onCeiling && m_StartDistance < 0.0f) {
        m_StartDistance = playerTotalDist;
        m_LastCheckedDistance = playerTotalDist;
    }

    // 在天花板上：累加本片段自上次检查到现在的里程差
    if (m_StartDistance >= 0.0f) {
        float delta = playerTotalDist - m_LastCheckedDistance;
        if (delta > 0.0f) {
            m_TotalCeilingDistance += delta;
            m_LastCheckedDistance = playerTotalDist;
            if (m_TotalCeilingDistance >= kTargetDistance) {
                m_TotalCeilingDistance = kTargetDistance;
                m_Completed = true;
            }
        }
    }
}

void Level9::Render() {}