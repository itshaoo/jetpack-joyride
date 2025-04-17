#ifndef COIN_MANAGER_HPP
#define COIN_MANAGER_HPP

#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "CoinGroup.hpp"
#include <vector>
#include <memory>

class CoinManager {
public:
    CoinManager();
    ~CoinManager() = default;

    // 每次更新：檢查是否 spawn 新群組
    void Update(float deltaTime);
    // 設定 Renderer，使生成的 CoinGroup 能加入 Renderer 統一渲染
    void SetRenderer(Util::Renderer* renderer) { m_Renderer = renderer; }
private:
    void SpawnCoinGroup();
    std::vector<std::shared_ptr<CoinGroup>> m_CoinGroups;
    float m_SpawnTimer;
    float m_SpawnInterval; // 隨機介於 4 至 6 秒
    Util::Renderer* m_Renderer = nullptr;
};

#endif // COIN_MANAGER_HPP