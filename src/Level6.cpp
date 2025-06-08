#include "Level6.hpp"
#include "App.hpp"
#include "Util/Text.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"

Level6::Level6(App* app)
: m_App(app)
{}

void Level6::Start() {
    m_App->Start();

    m_StartDistance    = -1.0f;
    m_StartCoinCount   = m_App->GetCollisionManager()->GetCoinCount();
    m_CurrentCoinCount = 0;
    m_Completed        = false;
}

void Level6::Update() {
    if (m_Completed) return;

    float dist = m_App->GetDistance();
    int   totalCoins = m_App->GetCollisionManager()->GetCoinCount();

    // 还没开始计数，等玩家跑到 1000m
    if (dist < kStartDistance) return;

    // 第一次到达 1000m，记录初始金币数
    if (m_StartDistance < 0.0f) {
        m_StartDistance  = dist;
        m_StartCoinCount = totalCoins;
    }

    // 在 [1000,1300]m 区间内，更新当前区间内的金币数
    if (dist <= kEndDistance) {
        m_CurrentCoinCount = totalCoins - m_StartCoinCount;
        if (m_CurrentCoinCount < 0) m_CurrentCoinCount = 0;
        if (m_CurrentCoinCount > kTargetCoins) m_CurrentCoinCount = kTargetCoins;
        // 足够则完成
        if (m_CurrentCoinCount >= kTargetCoins) {
            m_Completed = true;
            }
    }
}

void Level6::Render() {}