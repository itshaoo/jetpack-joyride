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

    // 如果需要的话，也可以在 Level4 里自己维护“行走距离”并暴露一个 getter
    int  GetCurrentWalkDistance() const { return static_cast<int>(m_CurrentWalkDistance); }
    int  GetTargetDistance()  const { return static_cast<int>(m_TargetDistance); }
private:
    App* m_App;

    float  m_TargetDistance     = 500.0f;
    // 累計的步行距離
    float  m_CurrentWalkDistance = 0.0f;
    // 上一幀 Player 的步行總距離
    float  m_LastPlayerWalkDist = 0.0f;

    bool  m_Completed      = false;
    float  m_LastDisplayDistance = 0.0f;
};

#endif // LEVEL4_HPP