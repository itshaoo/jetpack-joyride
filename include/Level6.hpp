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
private:
    App* m_App;
    float m_TargetLeastDistance = 1000.0f;
    float m_TargetMostDistance = 1300.0f;
    bool m_Completed = false;
};

#endif // LEVEL6_HPP