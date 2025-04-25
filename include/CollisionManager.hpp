#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include <vector>
#include <memory>
#include "Player.hpp"
#include "ZapperManager.hpp"
#include "CoinManager.hpp"
#include "Missile.hpp"
#include "Equipment.hpp"
#include "Util/BGM.hpp"

class CollisionManager {
public:
    CollisionManager(
        Player* player,
        ZapperManager* zapperMgr,
        CoinManager* coinMgr,
        std::vector<std::shared_ptr<Missile>>& missiles,
        std::vector<std::shared_ptr<Equipment>>& equipments
    );

    // 回傳 true 表示玩家死亡，遊戲應該立即結束
    bool Update();

    int GetCoinCount() const { return m_CoinCount; }

private:
    // 收縮後的 AABB 碰撞檢測
    bool CheckAABB(
        glm::vec2 posA, glm::vec2 sizeA,
        glm::vec2 posB, glm::vec2 sizeB
    ) const;

    // 收縮比例，0.0 ~ 0.5，越大命中框越小（敏感度越低）
    static constexpr float kInsetRatio = 0.15f;

    Player* m_Player;
    ZapperManager* m_ZapperMgr;
    CoinManager*  m_CoinMgr;
    std::vector<std::shared_ptr<Missile>>& m_Missiles;
    std::vector<std::shared_ptr<Equipment>>& m_Equipments;

    int m_CoinCount = 0;

    std::shared_ptr<Util::BGM> m_CoinSound;
};

#endif // COLLISIONMANAGER_HPP