#include "ZapperManager.hpp"
#include "config.hpp"
#include <cstdlib>
#include "Util/Time.hpp"

ZapperManager::ZapperManager(Util::Renderer* renderer, float bgSpeed)
  : m_Renderer(renderer), m_BackgroundSpeed(bgSpeed) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void ZapperManager::Update() {
    m_SpawnTimer += Util::Time::GetDeltaTimeMs();
    if (m_SpawnTimer >= m_SpawnInterval * 1000.0f) {
        SpawnZappers();
        m_SpawnTimer = 0.0f;
        m_SpawnInterval = 2.0f + (std::rand() % 3);
    }
    for (auto it = m_Zappers.begin(); it != m_Zappers.end();) {
        auto& zap = *it;
        zap->Update(m_BackgroundSpeed);
        if (zap->IsOffScreen(WINDOW_WIDTH)) {
            it = m_Zappers.erase(it);
        } else ++it;
    }
}

void ZapperManager::SpawnZappers() {
    int count = 3 + (std::rand()%3);
    float baseY = m_MinY + (std::rand()/(float)RAND_MAX)*(m_MaxY-m_MinY);
    float spawnX = (WINDOW_WIDTH/2) + 50.0f;
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
    for (int i=0; i<count; ++i) {
        bool vert = (std::rand()%2)==0;
        auto zap = std::make_shared<Zapper>(vert?pathsVert:pathsHor,
                                           glm::vec2(spawnX + i*200.0f, baseY));
        zap->AddToRenderer(*m_Renderer);
        m_Zappers.push_back(zap);
    }
}
