#ifndef ZAPPERMANAGER_HPP
#define ZAPPERMANAGER_HPP

#include <vector>
#include <memory>
#include "Zapper.hpp"
#include "Util/Renderer.hpp"

class ZapperManager {
public:
    ZapperManager(Util::Renderer* renderer,
                  float bgSpeed,
                  float minY = -230.0f,   // groundPosition.y
                float maxY =  230.0f); // maxHeight
    void Update(float backgroundSpeed);

    const std::vector<std::shared_ptr<Zapper>>& GetZappers() const {
        return m_Zappers;
    }

    void RemoveZapper(const std::shared_ptr<Zapper>& zapper);

    void SpawnZappers();

    // 僅更新位置並剔除畫面外
    void UpdateExisting(float backgroundSpeed);

private:

    std::vector<std::shared_ptr<Zapper>> m_Zappers;
    Util::Renderer* m_Renderer;
    float m_BackgroundSpeed;
    float m_MinY;
    float m_MaxY;
};

#endif