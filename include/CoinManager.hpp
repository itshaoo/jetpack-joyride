#ifndef COINMANAGER_HPP
#define COINMANAGER_HPP

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Coin.hpp"
#include "Util/Renderer.hpp"

class CoinManager {
public:
    CoinManager(Util::Renderer* renderer,
                float bgSpeed,
                float minY = -230.0f,   // groundPosition.y
                float maxY =  230.0f);
    void Update(float backgroundSpeed);

    std::vector<std::shared_ptr<Coin>>& GetCoins() { return m_Coins; }
    const std::vector<std::shared_ptr<Coin>>& GetCoins() const { return m_Coins; }
    void RemoveCoin(const std::shared_ptr<Coin>& coin) {
        // Remove the visual of the collected coin
        m_Renderer->RemoveChild(coin->GetAnimation());
    }

    void SpawnCoinPattern();

    void UpdateExisting(float backgroundSpeed);

private:

    Util::Renderer* m_Renderer;
    float m_BackgroundSpeed;
    float m_MinY;
    float m_MaxY;

    // Active coins in the scene
    std::vector<std::shared_ptr<Coin>> m_Coins;
};

#endif // COINMANAGER_HPP