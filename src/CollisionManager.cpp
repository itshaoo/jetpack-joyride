#include "CollisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager(
    Player* player,
    ZapperManager* zapperMgr,
    CoinManager* coinMgr,
    std::vector<std::shared_ptr<Missile>>& missiles,
    std::vector<std::shared_ptr<Equipment>>& equipments
) : m_Player(player)
  , m_ZapperMgr(zapperMgr)
  , m_CoinMgr(coinMgr)
  , m_Missiles(missiles)
  , m_Equipments(equipments)
  , m_CoinCount(0)
{
    // 初始化金幣音效
    m_CoinSound = std::make_shared<Util::BGM>(std::string(RESOURCE_DIR) + "/Sounds/coin_pickup.wav");
}

// 收縮 AABB 然後判定相交
bool CollisionManager::CheckAABB(
    glm::vec2 posA, glm::vec2 sizeA,
    glm::vec2 posB, glm::vec2 sizeB
) const {
    // 向內收縮
    glm::vec2 insetA = sizeA * kInsetRatio;
    glm::vec2 insetB = sizeB * kInsetRatio;
    posA += insetA;
    sizeA -= insetA * 2.0f;
    posB += insetB;
    sizeB -= insetB * 2.0f;

    // 權威的 AABB 相交測試
    return !(posA.x + sizeA.x < posB.x ||
             posB.x + sizeB.x < posA.x ||
             posA.y + sizeA.y < posB.y ||
             posB.y + sizeB.y < posA.y);
}

bool CollisionManager::Update() {
    glm::vec2 pPos  = m_Player->GetPosition();
    glm::vec2 pSize = m_Player->GetSize();

    // 1) Zapper 碰撞 → 結束
    for (auto& zap : m_ZapperMgr->GetZappers()) {
        if (CheckAABB(pPos, pSize, zap->GetPosition(), zap->GetSize())) {
            std::cout << "Player hit Zapper\n";
            return true;
        }
    }

    // 2) Missile 碰撞 → 結束
    for (auto& msl : m_Missiles) {
        if (CheckAABB(pPos, pSize, msl->GetPosition(), msl->GetSize())) {
            std::cout << "Player hit Missile\n";
            return true;
        }
    }

    // 3) Coin 撿取 → 累加，不結束
    {
        auto& coins = m_CoinMgr->GetCoins();
        for (auto it = coins.begin(); it != coins.end();) {
            auto& coin = *it;
            if (CheckAABB(pPos, pSize,coin->GetPosition(), coin->GetSize())) {
                if (m_CoinSound) m_CoinSound->Play(0);
                ++m_CoinCount;
                m_CoinMgr->RemoveCoin(coin);
                it = coins.erase(it);
            } else {
                ++it;
            }
        }
    }

    // 4) Equipment 碰撞 → 只輸出，不結束
    for (auto& eq : m_Equipments) {
        if (CheckAABB(pPos, pSize, eq->GetPosition(), eq->GetSize())) {
            std::cout << "Player touched Equipment\n";
        }
    }

    return false;
}