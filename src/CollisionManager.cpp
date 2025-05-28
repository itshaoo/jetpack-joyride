#include "CollisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager(
    Player* player,
    ZapperManager* zapperMgr,
    CoinManager* coinMgr,
    std::vector<std::shared_ptr<Missile>>& missiles,
    std::vector<std::shared_ptr<Equipment>>& equipments,
    Util::Renderer* renderer
) : m_Player(player)
  , m_ZapperMgr(zapperMgr)
  , m_CoinMgr(coinMgr)
  , m_Missiles(missiles)
  , m_Equipments(equipments)
  , m_CoinCount(0)
  , m_Renderer(renderer)
{
    // 初始化金幣音效
    m_CoinSound = std::make_shared<Util::SFX>(std::string(RESOURCE_DIR) + "/Sounds/coin_pickup.wav");
}

// 收縮 AABB 然後判定相交
bool CollisionManager::CheckAABB(
    glm::vec2 posA, glm::vec2 sizeA,
    glm::vec2 posB, glm::vec2 sizeB
) const {

    // 你可以根據需要調整這個 margin 值（以像素為單位）
    glm::vec2 marginA(10.0f, 10.0f);  // 物件 A 的內縮量：左右各 10 像素，上下各 10 像素
    glm::vec2 marginB(10.0f, 10.0f);  // 物件 B 的內縮量

    // 建立物件 A 的碰撞框
    glm::vec2 collisionPosA = posA + marginA;
    // 因為左右各空出 marginA.x，上下各空出 marginA.y，所以要減掉2倍 margin
    glm::vec2 collisionSizeA = sizeA - marginA * 2.0f;

    // 建立物件 B 的碰撞框
    glm::vec2 collisionPosB = posB + marginB;
    glm::vec2 collisionSizeB = sizeB - marginB * 2.0f;

    // 進行 AABB 碰撞檢測，只使用我們定義的碰撞框
    return !(collisionPosA.x + collisionSizeA.x < collisionPosB.x ||
             collisionPosB.x + collisionSizeB.x < collisionPosA.x ||
             collisionPosA.y + collisionSizeA.y < collisionPosB.y ||
             collisionPosB.y + collisionSizeB.y < collisionPosA.y);
}

bool CollisionManager::Update() {
    glm::vec2 pPos  = m_Player->GetPosition();
    glm::vec2 pSize = m_Player->GetSize();

    // 1) Zapper 碰撞
    for (auto it = m_ZapperMgr->GetZappers().begin(); it != m_ZapperMgr->GetZappers().end(); ) {
        auto& zap = *it;
        if (CheckAABB(pPos, pSize, zap->GetPosition(), zap->GetSize())) {
            if (m_Player->HasGravitySuit()) {
                m_Player->DisableGravitySuit();
                m_ZapperMgr->RemoveZapper(zap); // 用這個方法移除
                it = m_ZapperMgr->GetZappers().begin(); // 重新取得 begin()，避免 iterator 失效
                continue;
            } else if (m_Player->HasLilStomper()) {
                m_ZapperMgr->RemoveZapper(zap);
                it = m_ZapperMgr->GetZappers().begin();
                continue;
            } else {
                std::cout << "Player hit Zapper\n";
                return true;
            }
        }
        ++it;
    }

    // 2) Missile 碰撞
    for (auto it = m_Missiles.begin(); it != m_Missiles.end(); ) {
        auto& msl = *it;
        if (CheckAABB(pPos, pSize, msl->GetPosition(), msl->GetSize())) {
            if (m_Player->HasGravitySuit()) {
                m_Player->DisableGravitySuit();
                it = m_Missiles.erase(it); // 移除障礙物
                if (m_Renderer) {
                m_Renderer->RemoveChild(std::static_pointer_cast<Util::GameObject>(msl->GetAnimationPtr()));
            }
                continue;
            } else if (m_Player->HasLilStomper()) {
                m_Player->DisableLilStomper();
                it = m_Missiles.erase(it); // 移除障礙物
                if (m_Renderer) {
                m_Renderer->RemoveChild(std::static_pointer_cast<Util::GameObject>(msl->GetAnimationPtr()));
                continue;
            }
            // Fix: add missing closing brace for else-if
            } else {
                std::cout << "Player hit Missile\n";
                return true;
            }
        }
        ++it;
    }

    // 3) Coin 撿取 → 累加，不結束
    {
        auto& coins = m_CoinMgr->GetCoins();
        for (auto it = coins.begin(); it != coins.end(); ) {
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

    // 4) Equipment 碰撞 → 撞到就消失
    for (auto it = m_Equipments.begin(); it != m_Equipments.end(); ) {
        auto& eq = *it;
        if (CheckAABB(pPos, pSize, eq->GetPosition(), eq->GetSize())) {
            std::cout << "Player touched Equipment\n";
            // 隨機選擇一種裝備
            if (std::rand() % 2 == 0) {
                m_Player->EnableGravitySuit();
            } else {
                m_Player->EnableLilStomper();
            }
            if (m_Renderer) {
                m_Renderer->RemoveChild(eq->equipmentAnimation);
            }
            it = m_Equipments.erase(it);
            ++m_EquipCount;
        } else {
            ++it;
        }
    return false;
    }
    return false;
}