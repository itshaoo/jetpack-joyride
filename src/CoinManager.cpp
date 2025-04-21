#include "CoinManager.hpp"
#include "config.hpp"
#include "Util/Time.hpp"
#include <cstdlib>

CoinManager::CoinManager(Util::Renderer* renderer, float bgSpeed)
  : m_Renderer(renderer), m_BackgroundSpeed(bgSpeed) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void CoinManager::Update() {
    // 累積時間，達到間隔就 spawn 新圖案
    m_SpawnTimer += Util::Time::GetDeltaTimeMs();
    if (m_SpawnTimer >= m_SpawnInterval) {
        SpawnCoinPattern();
        m_SpawnTimer = 0.0f;
        // 隨機下一次間隔 3~6 秒
        m_SpawnInterval = (3 + std::rand() % 4) * 1000.0f;
    }

    // 更新所有硬幣位置，並移除出螢幕者
    for (auto it = m_Coins.begin(); it != m_Coins.end();) {
        auto& coin = *it;
        coin->Update(m_BackgroundSpeed);
        if (coin->IsOffScreen(WINDOW_WIDTH)) {
            it = m_Coins.erase(it);
        } else {
            ++it;
        }
    }
}

void CoinManager::SpawnCoinPattern() {
    // 定義兩種圖案：箭頭與矩形
    const float cell = 40.0f; // 格子大小
    std::vector<std::vector<std::string>> patterns = {
        // 圖案 A：向右箭頭
        {".....X....",
            "....XXX...",
            "...XXXXX..",
            "....XXX...",
            ".....X...."
        },
        // 圖案 B：3x8 矩形
        {
            "XXXXXXXX",
            "XXXXXXXX",
            "XXXXXXXX"
        }
    };
    // 選一個隨機圖案
    auto& map = patterns[std::rand() % patterns.size()];

    // 計算起始基準位置
    float spawnX = (WINDOW_WIDTH / 2) + 50.0f;
    float baseY = m_MinY + (std::rand()/(float)RAND_MAX) * (m_MaxY - m_MinY);

    // 繪製圖案
    for (size_t row = 0; row < map.size(); ++row) {
        for (size_t col = 0; col < map[row].size(); ++col) {
            if (map[row][col] == 'X') {
                glm::vec2 pos(
                    spawnX + col * cell,
                    baseY - (int(map.size())/2 - int(row)) * cell
                );
                // 使用原本的 coin animation 幀
                std::vector<std::string> frames = {
                    RESOURCE_DIR"/Image/Coin/coin1.png",
                    RESOURCE_DIR"/Image/Coin/coin2.png",
                    RESOURCE_DIR"/Image/Coin/coin3.png",
                    RESOURCE_DIR"/Image/Coin/coin4.png",
                    RESOURCE_DIR"/Image/Coin/coin5.png",
                    RESOURCE_DIR"/Image/Coin/coin6.png",
                    RESOURCE_DIR"/Image/Coin/coin7.png",
                    RESOURCE_DIR"/Image/Coin/coin8.png"
                };
                auto coin = std::make_shared<Coin>(frames, pos);
                coin->AddToRenderer(*m_Renderer);
                m_Coins.push_back(coin);
            }
        }
    }
}