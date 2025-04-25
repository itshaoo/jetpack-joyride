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

    std::vector<std::shared_ptr<Coin>>& GetCoins() { return m_Coins; }
    const std::vector<std::shared_ptr<Coin>>& GetCoins() const { return m_Coins; }
    void RemoveCoin(const std::shared_ptr<Coin>& coin) {
        // Remove the visual of the collected coin
        m_Renderer->RemoveChild(coin->GetAnimation());
    }

private:
    void SpawnCoinPattern();

    Util::Renderer* m_Renderer;
    float m_BackgroundSpeed;
    float m_SpawnTimer{0.0f};       // milliseconds
    float m_SpawnInterval{4000.0f}; // milliseconds
    float m_MinY{-265.5f}, m_MaxY{250.0f};

    // Active coins in the scene
    std::vector<std::shared_ptr<Coin>> m_Coins;
};

#endif // COINMANAGER_HPP