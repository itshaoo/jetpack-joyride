#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "pch.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "config.hpp"
#include <vector>
#include <Util/Time.hpp>

struct OverlayAnim {
    std::vector<Util::Image> frames;
    float intervalMs = 100.0f;          // 每帧间隔
    float timer = 0.0f;         // 累计时间
    size_t  currentFrame = 0;   // 当前帧索引
    // 红灯动画稍后才激活
};

class Background {
public:
    Background();

    void Update();
    void Render();

    // 設定背景速度
    void SetBackgroundSpeed(float speed) {
        backgroundSpeed = speed;
    }

    void NotifyLogoOffScreen() { m_RedLightActive = true; }

    std::vector<std::pair<glm::vec2, glm::vec2>> GetRedLightBounds() const;

private:
    enum class Phase {
        INITIAL,
        LOOPING
    } m_Phase;

    std::vector<Util::Image> m_InitialImages;
    std::vector<Util::Image> m_LoopingImages;

    // 新增：LOOPING 階段用的紅燈動畫、以及在一格裡的相對 offset
    struct LoopRedLight {
        OverlayAnim anim;
        glm::vec2   offset;  // 相對於該背景 tile 左下角的位移
    };
    std::vector<LoopRedLight> m_LoopRedLights;

    OverlayAnim recBoardAnim;
    OverlayAnim wallBoardAnim;
    OverlayAnim redLightAnim;

    bool m_RedLightActive = false;

    float m_RedLightTransition = 0.0f;

    float m_ScrollX;
    float m_ScaledWidth;

    float backgroundSpeed = 4.0f;

    std::vector<Util::Image> m_StartOverlays;

    void updateInitial();
    void updateLooping();

    void renderInitial();
    void renderLooping();

    float getTileWidth() const {
        return m_ScaledWidth;
    }
    // 畫面上紅燈顯示的縮放比例，與 renderLooping() 中 redScale 相同
    static constexpr float kRedScale = 0.7f;
};

#endif