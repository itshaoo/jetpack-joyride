#include "Level1.hpp"
#include "App.hpp"
#include "Util/Renderer.hpp"
#include "CoinManager.hpp"
#include "CoinCounter.hpp"
#include "Util/Text.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Keycode.hpp"
#include "Util/Input.hpp"

Level1::Level1(App* app)
: m_App(app)
{}

void Level1::Start() {
    // 初始化關卡
    m_App->Start();
    // 可自訂 UI、文字
}

void Level1::Update() {
    // 檢查完成條件
    int coins = m_App->GetCollisionManager()->GetCoinCount();
    if (!m_Completed && coins >= m_TargetCoins) {
        m_Completed = true;
    }
}

void Level1::Render() {
    // empty: 第一關暫時不需額外畫面
}