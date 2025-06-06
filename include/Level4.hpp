#ifndef LEVEL4_HPP
#define LEVEL4_HPP

#include "ILevel.hpp"
#include <vector>
#include "Util/Text.hpp"
#include <glm/vec2.hpp>
#include <memory>

class App;

class Level4 : public ILevel {
public:
    explicit Level4(App* app);
    ~Level4() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }

    // 新增：对外暴露红灯已触碰计数
    int GetRedTouchedCount() const { return m_RedTouchedCount; }

    // 如果需要的话，也可以在 Level4 里自己维护“行走距离”并暴露一个 getter
    float GetWalkDistance() const { return m_WalkDistance; }

    int   m_RedTouchedCount = 0;
private:
    App* m_App;

    int  m_TargetRedLight   = 10;
    std::vector<bool> m_PrevCollided;

    float m_WalkDistance    = 0.0f;

    float m_TargetDistance = 500.0f;
    bool  m_Completed      = false;
};

#endif // LEVEL4_HPP