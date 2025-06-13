#ifndef LEVEL6_HPP
#define LEVEL6_HPP

#include "ILevel.hpp"

class App;

class Level6 : public ILevel {
public:
    explicit Level6(App* app);
    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }
    int  GetCurrentCoinCount() const { return m_CurrentCoinCount; }
    static constexpr int kTargetCoins = 40;
private:
    App* m_App;
    static constexpr float kStartDistance = 800.0f;
    static constexpr float kEndDistance   = 1500.0f;
    float m_StartDistance   = -1.0f;
    int   m_StartCoinCount  = 0;
    int   m_CurrentCoinCount = 0;
    bool m_Completed = false;
};

#endif // LEVEL6_HPP