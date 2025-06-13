#ifndef DISTANCECOUNTER_HPP
#define DISTANCECOUNTER_HPP

#include "Util/GameObject.hpp"
#include "Object.hpp"
#include <vector>
#include <string>

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

    void SetDistance(int distance);

private:
    size_t digitCount;
    std::string imgDir;
    float m_baseX;
    float m_baseY;
    float m_spacing;
    float m_unitOffset;
    float m_digitScale;

    void rebuild(int distance);
};

#endif