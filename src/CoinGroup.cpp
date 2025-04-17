#include "CoinGroup.hpp"
#include "Coin.hpp"
#include "Util/Logger.hpp"
#include <cstdlib>

CoinGroup::CoinGroup(int patternType) {
    // 預設 CoinGroup 為世界物件，且沒有自己的 drawable，所以 m_Drawable 為 nullptr
    if (patternType == 0) {
        // 矩形排列：例如 9 列 x 3 行，共 27 個硬幣
        int cols = 9;
        int rows = 3;
        float hSpacing = 40.0f;
        float vSpacing = 40.0f;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                auto coin = std::make_shared<Coin>();
                // 儲存相對於群組原點的位置
                coin->SetPosition({ c * hSpacing, r * vSpacing });
                AddChild(coin);
            }
        }
    } else {
        // 階梯排列：例如第一行 6 個，第二行 4 個，第三行 2 個，每行向右偏移
        int rowCoinCount[3] = {6, 4, 2};
        float hSpacing = 40.0f;
        float vSpacing = 40.0f;
        for (int r = 0; r < 3; ++r) {
            int cols = rowCoinCount[r];
            float offsetX = r * 20.0f;  // 每列向右偏移 20 像素
            for (int c = 0; c < cols; ++c) {
                auto coin = std::make_shared<Coin>();
                coin->SetPosition({ offsetX + c * hSpacing, r * vSpacing });
                AddChild(coin);
            }
        }
    }
}