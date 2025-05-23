#include "CollisionManager.hpp"
#include <iostream>

CollisionManager::CollisionManager(
    Player* player,
    ZapperManager* zapperMgr,
    CoinManager* coinMgr,
    std::vector<std::shared_ptr<Missile>>& missiles,
    std::vector<std::shared_ptr<Equipment>>& equipments,
    Util::Renderer* renderer // 新增這個參數
) : m_Player(player)
  , m_ZapperMgr(zapperMgr)
  , m_CoinMgr(coinMgr)
  , m_Missiles(missiles)
  , m_Equipments(equipments)
  , m_CoinCount(0)
  , m_Renderer(renderer) // 新增這個初始化
{
    // 初始化金幣音效
    m_CoinSound = std::make_shared<Util::BGM>(std::string(RESOURCE_DIR) + "/Sounds/coin_pickup.wav");
}

// 收縮 AABB 然後判定相交
bool CollisionManager::CheckAABB(
    glm::vec2 posA, glm::vec2 sizeA,
    glm::vec2 posB, glm::vec2 sizeB
) const {

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
        } else {
            ++it;
        }
    return false;
}

    return false;
}