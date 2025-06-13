#include "ZapperManager.hpp"
#include "config.hpp"
#include <cstdlib>
#include "Util/Time.hpp"
#include <algorithm>

ZapperManager::ZapperManager(Util::Renderer* renderer,
                             float bgSpeed,
                             float minY,
                             float maxY)
  : m_Renderer(renderer),
    m_BackgroundSpeed(bgSpeed),
    m_MinY(minY),
    m_MaxY(maxY)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void ZapperManager::Update(float backgroundSpeed) {
    // 使用 backgroundSpeed 更新 Zapper 位置
    for (auto it = m_Zappers.begin(); it != m_Zappers.end();) {
        auto& zap = *it;
        zap->Update(backgroundSpeed); // 使用傳入的背景速度
        if (zap->IsOffScreen(WINDOW_WIDTH)) {
            it = m_Zappers.erase(it);
        } else ++it;
    }
}

void ZapperManager::SpawnZappers() {
    int count = 3 + (std::rand()%3);
    float spawnX = (WINDOW_WIDTH/2) + 50.0f;
    float spacingX = 700.0f;                      // 每个 zapper 之间的水平间隔
    std::vector<std::string> pathsVert = {
        RESOURCE_DIR"/Image/Zapper/ver zapper1.png",
        RESOURCE_DIR"/Image/Zapper/ver zapper2.png",
        RESOURCE_DIR"/Image/Zapper/ver zapper3.png",
        RESOURCE_DIR"/Image/Zapper/ver zapper4.png"
    };
    std::vector<std::string> pathsHor = {
        RESOURCE_DIR"/Image/Zapper/zapper1.png",
        RESOURCE_DIR"/Image/Zapper/zapper2.png",
        RESOURCE_DIR"/Image/Zapper/zapper3.png",
        RESOURCE_DIR"/Image/Zapper/zapper4.png"
    };

    for (int i = 0; i < count; ++i) {
        int type = std::rand() % 2;
        const auto& chosenPaths = (type == 0 ? pathsVert : pathsHor);

        float y = m_MinY + (std::rand() / (float)RAND_MAX) * (m_MaxY - m_MinY);

        float x = spawnX + i * spacingX;

        auto zap = std::make_shared<Zapper>(chosenPaths, glm::vec2{x, y});
        zap->AddToRenderer(*m_Renderer);
        m_Zappers.push_back(zap);
    }
}

void ZapperManager::UpdateExisting(float backgroundSpeed) {
    for (auto it = m_Zappers.begin(); it != m_Zappers.end();) {
        auto& zap = *it;
        zap->Update(backgroundSpeed);
        if (zap->IsOffScreen(WINDOW_WIDTH)) {
            it = m_Zappers.erase(it);
        } else {
            ++it;
        }
    }
}

void ZapperManager::RemoveZapper(const std::shared_ptr<Zapper>& zapper) {
    auto& zappers = m_Zappers;
    // 先從 renderer 移除動畫
    if (m_Renderer) {
        m_Renderer->RemoveChild(std::static_pointer_cast<Util::GameObject>(zapper->GetAnimationPtr()));
    }
    // 再從容器移除
    zappers.erase(std::remove(zappers.begin(), zappers.end(), zapper), zappers.end());
}