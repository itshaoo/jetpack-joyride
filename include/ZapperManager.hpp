#ifndef ZAPPERMANAGER_HPP
#define ZAPPERMANAGER_HPP

#include <vector>
#include <memory>
#include "Zapper.hpp"
#include "Util/Renderer.hpp"

class ZapperManager {
public:
    ZapperManager(Util::Renderer* renderer, float bgSpeed);
    void Update();

    const std::vector<std::shared_ptr<Zapper>>& GetZappers() const {
        return m_Zappers;
    }

private:
    void SpawnZappers();

    std::vector<std::shared_ptr<Zapper>> m_Zappers;
    Util::Renderer* m_Renderer;
    float m_BackgroundSpeed;
    float m_SpawnTimer{0.0f};
    float m_SpawnInterval{3.0f};
    float m_MinY{-265.5f}, m_MaxY{250.0f};
};

#endif