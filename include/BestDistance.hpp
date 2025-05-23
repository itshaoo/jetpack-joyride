#ifndef BESTDISTANCE_HPP
#define BESTDISTANCE_HPP

#include "Util/GameObject.hpp"
#include "Object.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

class BestDistance : public Util::GameObject {
public:
    BestDistance(
        float baseX = -555.7f,
        float baseY = 231.0f,
        float spacing = 15.0f,
        float unitOffset = 0.4f,
        float digitScale = 0.5f,
        int zOrder = 10,
        float bestOffsetX = -603.0f, // 新增的參數，用來控制 Best 的 X 偏移
        float bestOffsetY = 231.0f,  // 新增的參數，用來控制 Best 的 Y 偏移
        float bestScale = 1.0f     // Best 圖像的縮放比例
    );

    void SetBestDistance(int bestDistance);

private:
    size_t digitCount;
    std::string imgDir;
    float m_baseX;
    float m_baseY;
    float m_spacing;
    float m_unitOffset;
    float m_digitScale;

    // 新增 Best 位置的偏移量
    float m_bestOffsetX;
    float m_bestOffsetY;
    float m_bestScale;   // Best 圖像的縮放比例

    void rebuild(int bestDistance);
};

#endif // BESTDISTANCE_HPP