#include "Level7.hpp"
#include "App.hpp"
#include "Util/Renderer.hpp"
#include "CoinManager.hpp"
#include "CoinCounter.hpp"
#include "Util/Text.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"

Level7::Level7(App* app)
: m_App(app)
{}

void Level7::Start() {
    // 初始化關卡
    m_App->Start();
    // 可自訂 UI、文字
}

void Level7::Update() {
    // 檢查完成條件
    float dist = m_App->GetDistance();  // 或者你加個 App::GetDistance()
    int coins = m_App->GetCollisionManager()->GetCoinCount();
    if (!m_Completed && dist >= m_TargetDistance && coins == 0) {
        m_Completed = true;
    }
}

void Level7::Render() {
    // empty: 第二關暫時不需額外畫面
}