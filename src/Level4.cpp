#include "Level4.hpp"
#include "App.hpp"
#include "Background.hpp"
#include "Player.hpp"

#include "Util/Text.hpp"
#include "Util/TransformUtils.hpp"

Level4::Level4(App* app)
  : m_App(app)
{}

void Level4::Start() {
    m_App->Start();

    Background* bg = m_App->GetBackground();
    auto bounds = bg->GetRedLightBounds();
    m_PrevCollided.assign(bounds.size(), false);

    m_RedTouchedCount = 0;
    m_Completed       = false;
}

void Level4::Update() {
    if (m_Completed) return;

    Background* bg  = m_App->GetBackground();
    Player*     ply = m_App->GetPlayer();

    auto bounds = bg->GetRedLightBounds();
    size_t n = bounds.size();
    if (m_PrevCollided.size() != n) {
        m_PrevCollided.assign(n, false);
    }

    glm::vec2 pPos  = ply->GetPosition();
    glm::vec2 pSize = ply->GetSize();

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
        else if (!collidingNow) {
            m_PrevCollided[i] = false;
        }
    }

    float walkDist = m_App->GetDistance();

    if (m_RedTouchedCount >= m_TargetRedLight && walkDist >= m_TargetDistance) {
        m_Completed = true;
    }
}

void Level4::Render() {
}