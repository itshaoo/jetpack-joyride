#include "DistanceCounter.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

DistanceCounter::DistanceCounter(
    float baseX,
    float baseY,
    float spacing,
    float unitOffset,
    float digitScale,
    int zOrder
) : Util::GameObject(nullptr, zOrder),
    digitCount(4),
    imgDir(std::string(RESOURCE_DIR) + "/Image/Count/"),
    m_baseX(baseX),
    m_baseY(baseY),
    m_spacing(spacing),
    m_unitOffset(unitOffset),
    m_digitScale(digitScale)
{
    SetZIndex(6.0f);
    // 初始顯示 0 米
    rebuild(0);
}

void DistanceCounter::SetDistance(int distance) {
    int needed = (distance == 0 ? 1 : static_cast<int>(std::log10(distance)) + 1);
    if (static_cast<size_t>(needed) > digitCount) {
        digitCount = needed;
    }
    rebuild(distance);
}

void DistanceCounter::rebuild(int distance) {
    // 清除舊圖示
    m_Children.clear();

    // 將距離轉為左補零字串
    std::ostringstream oss;
    oss << std::setw(digitCount) << std::setfill('0') << distance;
    std::string s = oss.str();

    // 建立每個數字子物件
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        std::string path = imgDir + "g" + c + ".png";
        auto digit = std::make_shared<Object>(path);
        digit->SetPosition({ m_baseX + static_cast<float>(i) * m_spacing, m_baseY });
        // 可設定縮放
        digit->m_Transform.scale = { m_digitScale, m_digitScale };
        digit->SetZIndex(6.0f);
        AddChild(digit);
    }

    // 最後加上單位 M
    auto mImg = std::make_shared<Object>(imgDir + "M.png");
    mImg->SetPosition({ m_baseX + static_cast<float>(s.size()) * m_spacing + m_unitOffset, m_baseY });
    mImg->m_Transform.scale = { m_digitScale, m_digitScale };
    mImg->SetZIndex(6.0f);
    AddChild(mImg);
}