#ifndef LEVEL9_HPP
#define LEVEL9_HPP

#include "ILevel.hpp"

class App;

class Level9 : public ILevel {
public:
    explicit Level9(App* app);
    ~Level9() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }

    // 給 App 暫停畫面呼叫，取得目前累積在天花板的距離
    float GetCurrentCeilingDistance() const { return m_TotalCeilingDistance; }

private:
    App* m_App;

    // 當玩家第一次進入「天花板跑步」時，紀錄當下的總里程
    float m_StartDistance = -1.0f;

    float m_LastCheckedDistance = 0.0f;
    float m_TotalCeilingDistance = 0.0f;

    static constexpr float kTargetDistance = 300.0f;
    bool m_Completed = false;
};

#endif // LEVEL9_HPP