#include "ZapperManager.hpp"
#include "Camera.hpp"
#include "config.hpp"   // 定義 WINDOW_WIDTH、RESOURCE_DIR
#include <cstdlib>
#include <ctime>
#include <iostream>

ZapperManager::ZapperManager()
    : m_SpawnTimer(0.0f),
      m_SpawnInterval(1.0f), // 初始間隔 1 秒
      m_MinY(-265.5f),
      m_MaxY(250.0f)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void ZapperManager::Update(float deltaTime) {
    // 1) 累積時間，到達間隔就生成新障礙物
    m_SpawnTimer += deltaTime;
    if (m_SpawnTimer >= m_SpawnInterval) {
        SpawnZappers();
        m_SpawnTimer = 0.0f;
        // 隨機下一次間隔 2~5 秒
        m_SpawnInterval = 2.0f + static_cast<float>(std::rand()) / (RAND_MAX / 3.0f);
    }

    // 2) 移除已經滾出畫面左側的 Zapper
    //    判斷條件：障礙物的世界 X 若低於 (攝影機 offset - 一半視窗寬度 - margin)
    float leftBoundary = Camera::GetInstance().GetOffset().x - (WINDOW_WIDTH / 2) - 100.0f;
    for (auto it = m_Zappers.begin(); it != m_Zappers.end();) {
        if ((*it)->GetPosition().x < leftBoundary) {
            it = m_Zappers.erase(it);
        } else {
            ++it;
        }
    }
}

void ZapperManager::Render() {
    // 如果你需要額外繪製（例如 Debug 框），可以在這裡做
    // 否則所有 Zapper 已經由 m_Renderer 管理，不必在這裡再 Draw()
}

void ZapperManager::SpawnZappers() {
    int count = 3 + std::rand() % 3;  // 隨機生成 3~5 個
    // 隨機 Y 座標
    float baseY = m_MinY + (static_cast<float>(std::rand()) / RAND_MAX) * (m_MaxY - m_MinY);
    // 世界 X：攝影機當前偏移 + 半個視窗寬 + margin
    float spawnX = Camera::GetInstance().GetOffset().x + (WINDOW_WIDTH / 2) + 50.0f;
    float spacing = 200.0f;  // 障礙物之間水平間距

    for (int i = 0; i < count; ++i) {
        ZapperType type = (std::rand() % 2 == 0
                           ? ZapperType::VERTICAL
                           : ZapperType::HORIZONTAL);

        // 計算這一隻的座標
        glm::vec2 pos(spawnX + i * spacing, baseY);

        // 建立 Zapper，構造內會自動依 type 載入對應動畫
        auto zapper = std::make_shared<Zapper>(type, pos);

        // 加入管理列表，並注入 Renderer（由外部負責繪製）
        m_Zappers.push_back(zapper);
        if (m_Renderer) {
            m_Renderer->AddChild(zapper);
        }
    }
}