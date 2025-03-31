#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "pch.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "config.hpp"
#include <vector>

class Background {
public:
    Background();

    void Update();
    void Render();

private:
    enum class Phase {
        INITIAL,
        LOOPING
    } m_Phase;

    std::vector<Util::Image> m_InitialImages;
    std::vector<Util::Image> m_LoopingImages;

    float m_ScrollX;
    float m_ScaledWidth;

    void updateInitial();
    void updateLooping();

    void renderInitial();
    void renderLooping();
};

#endif
