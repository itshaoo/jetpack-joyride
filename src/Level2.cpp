#include "Level2.hpp"
#include "App.hpp"
#include "Util/Renderer.hpp"
#include "CoinManager.hpp"
#include "CoinCounter.hpp"
#include "Util/Text.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"

Level2::Level2(App* app)
: m_App(app)
{}

void Level2::Start() {
    // 初始化關卡
    m_App->Start();
    // 可自訂 UI、文字
}

void Level2::Update() {
    // 檢查完成條件
    float dist = m_App->GetDistance();  // 或者你加個 App::GetDistance()
    if (!m_Completed && dist >= m_TargetDistance) {
        m_Completed = true;
    }
}

void Level2::Render() {
    // empty: 第二關暫時不需額外畫面
}