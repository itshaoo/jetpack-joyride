#ifndef LEVEL2_HPP
#define LEVEL2_HPP

#include "ILevel.hpp"

class App;

class Level2 : public ILevel {
public:
    explicit Level2(App* app);
    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }
private:
    App* m_App;
    float m_TargetDistance = 1000.0f;
    bool m_Completed = false;
};

#endif // LEVEL2_HPP