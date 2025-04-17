#include "CoinManager.hpp"
#include "config.hpp"
#include "Camera.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Animation.hpp"

CoinManager::CoinManager()
    : m_SpawnTimer(0.0f),
      m_SpawnInterval(4.0f) // 初始設為每 4 秒 spawn 一個群組
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void CoinManager::Update(float deltaTime) {
    m_SpawnTimer += deltaTime;
    if (m_SpawnTimer >= m_SpawnInterval) {
        SpawnCoinGroup();
        m_SpawnTimer = 0.0f;
        // 隨機下一次 spawn 介於 4 到 6 秒
        m_SpawnInterval = 4.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
        std::cout << "Next CoinGroup spawn interval: " << m_SpawnInterval << std::endl;
    }
    // 移除超出螢幕左側的 coin group (以世界座標判斷)
    for (auto it = m_CoinGroups.begin(); it != m_CoinGroups.end();) {
        if ((*it)->GetTransform().translation.x < Camera::GetInstance().GetOffset().x - WINDOW_WIDTH / 2 - 100) {
            it = m_CoinGroups.erase(it);
        } else {
            ++it;
        }
    }
}

void CoinManager::SpawnCoinGroup() {
    // 隨機排列模式：0 表示矩形，1 表示階梯
    int pattern = rand() % 2;
    auto group = std::make_shared<CoinGroup>(pattern);
    // 計算群組 spawn 的世界位置：
    float margin = 50.0f;
    // 以目前攝影機 offset 計算右邊界的世界 X：螢幕中心為 0
    float spawnX = Camera::GetInstance().GetOffset().x + (WINDOW_WIDTH / 2) + margin;
    // 隨機 Y 介於 -265.5 到 250
    float baseY = -265.5f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (250.0f - (-265.5f));
    // 將 CoinGroup 的 transform 設為 spawn 位置
    group->m_Transform.translation = {spawnX, baseY};
    // 將內部各硬幣的相對位置轉為絕對位置：
    for (auto &child : group->GetChildren()) {
        // 將 child 轉型為 Animation 類別，因為 Coin 是基於 Animation 的
        auto animChild = std::dynamic_pointer_cast<Animation>(child);
        if (animChild) {
            animChild->SetPosition(animChild->GetPosition() + group->m_Transform.translation);
        }
    }
    // 重設群組自身 transform 為 {0,0}，不影響硬幣的絕對位置
    group->m_Transform.translation = {0, 0};

    std::cout << "Spawned CoinGroup at (" << spawnX << ", " << baseY << ") with pattern " << pattern << std::endl;
    m_CoinGroups.push_back(group);
    if (m_Renderer) {
        m_Renderer->AddChild(group);
    }
}