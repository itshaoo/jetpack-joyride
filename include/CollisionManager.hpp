#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include <vector>
#include <memory>
#include "Player.hpp"
#include "ZapperManager.hpp"
#include "CoinManager.hpp"
#include "missile.hpp"
#include "Equipment.hpp"
#include "Util/SFX.hpp"
#include <Util/Renderer.hpp>
#include "Animation.hpp"

class CollisionManager {
public:
    CollisionManager(
        App* app,
        Player* player,
        ZapperManager* zapperMgr,
        CoinManager* coinMgr,
        std::vector<std::shared_ptr<Missile>>& missiles,
        std::vector<std::shared_ptr<Equipment>>& equipments,
        Util::Renderer* renderer
    );

    bool Update();

    int GetCoinCount() const { return m_CoinCount; }

    int GetEquipmentCount() const { return m_EquipCount; }

private:
    App* m_App;
    // 收縮後的 AABB 碰撞檢測
    bool CheckAABB(
        glm::vec2 posA, glm::vec2 sizeA,
        glm::vec2 posB, glm::vec2 sizeB
    ) const;

    Player* m_Player;
    ZapperManager* m_ZapperMgr;
    CoinManager*  m_CoinMgr;
    std::vector<std::shared_ptr<Missile>>& m_Missiles;
    std::vector<std::shared_ptr<Equipment>>& m_Equipments;

    int m_CoinCount = 0;

    std::shared_ptr<Util::SFX> m_CoinSound;

    Util::Renderer* m_Renderer;

    int m_EquipCount = 0;

    // 撿到金幣後的閃光動畫暫存
    std::vector<std::shared_ptr<Animation>> m_ShineAnims;
};

#endif // COLLISIONMANAGER_HPP