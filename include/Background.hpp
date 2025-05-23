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

private:
    enum class Phase {
        INITIAL,
        LOOPING
    } m_Phase;

    std::vector<Util::Image> m_InitialImages;
    std::vector<Util::Image> m_LoopingImages;

    OverlayAnim recBoardAnim;
    OverlayAnim wallBoardAnim;
    OverlayAnim redLightAnim;

    bool m_RedLightActive = false;

    float m_ScrollX;
    float m_ScaledWidth;

    float backgroundSpeed = 4.0f;

    std::vector<Util::Image> m_StartOverlays;

    void updateInitial();
    void updateLooping();

    void renderInitial();
    void renderLooping();
};

#endif