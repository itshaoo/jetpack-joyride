#include "CoinCounter.hpp"
#include "config.hpp"
#include <sstream>
#include <iomanip>

CoinCounter::CoinCounter() {
    SetCount(0);
}

void CoinCounter::SetCount(int count) {
    // 移除舊子物件
    m_Children.clear();

    // 轉為字串：至少三位，不限上限
    std::string s;
    if (count < 1000) {
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << count;
        s = oss.str();
    } else {
        s = std::to_string(count);
    }

    // 調整區域：可自行修改下列常數
    constexpr float baseX = -620.0f;
    constexpr float baseY = 200.0f;
    constexpr float spacing = 20.0f;
    constexpr float iconOffset = 15.0f;

    // 動態產生數字圖示
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        std::string path = std::string(RESOURCE_DIR) + "/Image/Count/" + c + ".png";
        auto digitImg = std::make_shared<Object>(path);
        digitImg->SetPosition({ baseX + i * spacing, baseY });
        AddChild(digitImg);
    }

    // 最後加上金幣圖示
    std::string coinPath = std::string(RESOURCE_DIR) + "/Image/Count/coin.png";
    auto coinImg = std::make_shared<Object>(coinPath);
    coinImg->SetPosition({ baseX + s.size() * spacing + iconOffset, baseY });
    AddChild(coinImg);
}