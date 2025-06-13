#ifndef LEVEL5_HPP
#define LEVEL5_HPP

#include "ILevel.hpp"

class App;

class Level5 : public ILevel {
public:
    explicit Level5(App* app);
    ~Level5() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }

    float GetCurrentFlightDistance() const { return m_CurrentFlightDistance; }

private:
    App*   m_App;

    // 玩家離地開始那一刻的總距離
    float  m_StartDistance = -1.0f;
    // 玩家在空中累積的飛行距離
    float  m_CurrentFlightDistance = 0.0f;

    static constexpr float kTargetFlight = 500.0f;

    bool   m_Completed = false;
};

#endif // LEVEL5_HPP