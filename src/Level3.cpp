#include "Level3.hpp"
#include "App.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "config.hpp" // for WINDOW_WIDTH

Level3::Level3(App* app)
  : m_App(app)
{}

void Level3::Start() {
    m_App->Start();

    // 初始化循环红灯碰撞状态
    auto bounds = m_App->GetBackground()->GetRedLightBounds();
    m_PrevCollided.assign(bounds.size(), false);

    m_RedTouchedCount     = 0;
    m_Completed           = false;
    m_PrevInitialCollided = false;
}

void Level3::Update() {
    if (m_Completed) return;

    auto* bg  = m_App->GetBackground();
    auto* ply = m_App->GetPlayer();
    glm::vec2 pPos  = ply->GetPosition();
    glm::vec2 pSize = ply->GetSize();

    // 初始畫面的紅燈
    {
        auto [initPos, initSize] = bg->GetInitialRedLightBounds();
        if (initSize.x > 0.0f && initSize.y > 0.0f) {
            bool collidingInit = !(
                (pPos.x + pSize.x) < initPos.x ||
                (initPos.x + initSize.x) < pPos.x ||
                (pPos.y + pSize.y) < initPos.y ||
                (initPos.y + initSize.y) < pPos.y
            );
            if (collidingInit && !m_PrevInitialCollided) {
                m_RedTouchedCount++;
                m_PrevInitialCollided = true;
            }
        }
    }

    // 循環紅燈
    auto bounds = bg->GetRedLightBounds();
    size_t n = bounds.size();
    if (m_PrevCollided.size() != n) {
        m_PrevCollided.assign(n, false);
    }

    for (size_t i = 0; i < n; ++i) {
        const auto& [rPos, rSize] = bounds[i];
        bool collidingNow = !(
            (pPos.x + pSize.x) < rPos.x ||
            (rPos.x + rSize.x) < pPos.x ||
            (pPos.y + pSize.y) < rPos.y ||
            (rPos.y + rSize.y) < pPos.y
        );
        if (collidingNow && !m_PrevCollided[i]) {
            m_RedTouchedCount++;
            m_PrevCollided[i] = true;
        }
        else if (!collidingNow && m_PrevCollided[i]) {
            // 僅在該紅燈完全滑出螢幕後，才重置狀態以便下一輪計數
            if (rPos.x + rSize.x < -(WINDOW_WIDTH / 2.0f)) {
                m_PrevCollided[i] = false;
            }
        }
    }

    if (m_RedTouchedCount >= m_TargetRedLight) {
        m_Completed = true;
    }
}

void Level3::Render() {}