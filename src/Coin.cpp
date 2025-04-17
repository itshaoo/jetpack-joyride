#include "Coin.hpp"
#include "config.hpp" // 使用 RESOURCE_DIR

Coin::Coin()
    : Animation({
          RESOURCE_DIR "/Image/Coin/coin1.png",
          RESOURCE_DIR "/Image/Coin/coin2.png",
          RESOURCE_DIR "/Image/Coin/coin3.png",
          RESOURCE_DIR "/Image/Coin/coin4.png",
          RESOURCE_DIR "/Image/Coin/coin5.png",
          RESOURCE_DIR "/Image/Coin/coin6.png",
          RESOURCE_DIR "/Image/Coin/coin7.png",
          RESOURCE_DIR "/Image/Coin/coin8.png"
      })
{
    // 設定硬幣動畫屬性：持續循環、每 100 毫秒換一幀
    SetLooping(true);
    SetInterval(100);
    // Coin 物件預設為世界物件（不須修改 m_IsWorldObject）

    Play();
}