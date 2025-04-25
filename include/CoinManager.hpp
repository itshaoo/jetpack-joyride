#ifndef COINMANAGER_HPP
#define COINMANAGER_HPP

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Coin.hpp"
#include "Util/Renderer.hpp"

class CoinManager {
public:
    CoinManager(Util::Renderer* renderer, float bgSpeed);
    void Update();

private:
    void SpawnCoinPattern();

    Util::Renderer* m_Renderer;
    float m_BackgroundSpeed;
    float m_SpawnTimer{0.0f};       // 毫秒
    float m_SpawnInterval{4000.0f}; // 毫秒
    float m_MinY{-265.5f}, m_MaxY{250.0f};

    // 已 spawn 的硬幣
    std::vector<std::shared_ptr<Coin>> m_Coins;
};

#endif