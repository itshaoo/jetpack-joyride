#ifndef COINCOUNTER_HPP
#define COINCOUNTER_HPP

#include "Util/GameObject.hpp"
#include "Object.hpp"

/**
 * @class CoinCounter
 * @brief HUD 顯示當前已撿到的金幣數量
 */
class CoinCounter : public Util::GameObject {
public:
    CoinCounter();

    /**
     * @brief 更新顯示的金幣總數
     * @param count 當前金幣數量
     */
    void SetCount(int count);

    // 可透過 GameObject::SetVisible 控制顯示/隱藏
};

#endif