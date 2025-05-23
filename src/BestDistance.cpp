#include "BestDistance.hpp"

BestDistance::BestDistance(
    float baseX,
    float baseY,
    float spacing,
    float unitOffset,
    float digitScale,
    int zOrder,
    float bestOffsetX,
    float bestOffsetY,
    float bestScale
) : Util::GameObject(nullptr, zOrder),
    digitCount(4),
    imgDir(std::string(RESOURCE_DIR) + "/Image/Count/"),
    m_baseX(baseX),
    m_baseY(baseY),
    m_spacing(spacing),
    m_unitOffset(unitOffset),
    m_digitScale(digitScale),
    m_bestOffsetX(bestOffsetX),
    m_bestOffsetY(bestOffsetY),
    m_bestScale(bestScale)
{
    SetZIndex(6.0f);
    // 從 best.txt 讀取最佳距離並顯示
    std::ifstream file(RESOURCE_DIR "/best.txt");
    int bestDistance = 0;
    if (file.is_open()) {
        file >> bestDistance;
    }
    file.close();
    rebuild(bestDistance);
}

void BestDistance::SetBestDistance(int bestDistance) {
    rebuild(bestDistance);
}

void BestDistance::rebuild(int bestDistance) {
    m_Children.clear();

    // 將距離轉為左補零字串
    std::ostringstream oss;
    oss << std::setw(digitCount) << std::setfill('0') << bestDistance;
    std::string s = oss.str();

    // 顯示 "Best" 的圖片並設置其位置，使用 m_bestOffsetX 和 m_bestOffsetY 來調整
    auto bestImg = std::make_shared<Object>(imgDir + "best.png");
    bestImg->SetPosition({ m_bestOffsetX, m_bestOffsetY });
    bestImg->m_Transform.scale = { m_bestScale, m_bestScale };  // 這裡使用 m_bestScale 控制大小
    bestImg->SetZIndex(6.0f);
    AddChild(bestImg);

    // 顯示每個數字
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        std::string path = imgDir + "g" + c + ".png";
        auto digit = std::make_shared<Object>(path);
        digit->SetPosition({ m_baseX + static_cast<float>(i) * m_spacing, m_baseY });
        digit->m_Transform.scale = { m_digitScale, m_digitScale };
        digit->SetZIndex(6.0f);
        AddChild(digit);
    }
}