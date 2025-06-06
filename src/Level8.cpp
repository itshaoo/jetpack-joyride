#include "Level8.hpp"
#include "App.hpp"
#include "CollisionManager.hpp"

Level8::Level8(App* app)
  : m_App(app)
{}

void Level8::Start() {
    // 调用通用的游戏启动
    m_App->Start();
}

void Level8::Update() {
    // 从 CollisionManager 里拿装备数量
    int collected = m_App->GetCollisionManager()->GetEquipmentCount();
    // （需要先在 CollisionManager 中新增 GetEquipmentCount()，见下方步骤）
    if (!m_Completed && collected >= m_EquipTarget) {
        m_Completed = true;
    }
}

void Level8::Render() {
    // 不额外绘制
}