#ifndef DISTANCECOUNTER_HPP
#define DISTANCECOUNTER_HPP

#include "Util/GameObject.hpp"
#include "Object.hpp"
#include <vector>
#include <string>

/**
 * @class DistanceCounter
 * @brief HUD 顯示玩家距離，使用圖示貼圖 0~9、M
 */
class DistanceCounter : public Util::GameObject {
public:
    DistanceCounter(
        float baseX = -617.7f,
        float baseY = 265.0f,
        float spacing = 24.0f,
        float unitOffset = 0.4f,
        float digitScale = 0.9f,
        int zOrder = 10
    );

    /**
     * @brief 設定距離（整數米），自動更新顯示
     */
    void SetDistance(int distance);

private:
    size_t digitCount;
    std::string imgDir;
    float m_baseX;
    float m_baseY;
    float m_spacing;
    float m_unitOffset;
    float m_digitScale;

    /**
     * @brief 重新建構子物件
     */
    void rebuild(int distance);
};

#endif