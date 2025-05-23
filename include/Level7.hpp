#ifndef LEVEL7_HPP
#define LEVEL7_HPP

#include "ILevel.hpp"

class App;

class Level7 : public ILevel {
public:
    explicit Level7(App* app);
    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }
private:
    App* m_App;
    float m_TargetDistance = 1000.0f;
    int m_TargetCoins = 0;
    bool m_Completed = false;
};

#endif // LEVEL7_HPP