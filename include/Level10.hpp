#ifndef LEVEL10_HPP
#define LEVEL10_HPP

#include "ILevel.hpp"

class App;

class Level10 : public ILevel {
public:
    explicit Level10(App* app);
    ~Level10() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }

    // 暫停頁面要呼叫，用以顯示目前累積距離
    float GetCurrentVehicleDistance() const { return m_CurrentVehicleDistance; }

private:
    App* m_App;

    // 當玩家剛穿上 Vehicle 時，記錄「那一刻的總里程」
    float m_StartDistance = -1.0f;

    // 從穿上之後累積的距離：Player::GetDistance() - m_StartDistance
    float m_CurrentVehicleDistance = 0.0f;

    static constexpr float kTargetDistance = 700.0f;
    bool m_Completed = false;
};

#endif // LEVEL10_HPP