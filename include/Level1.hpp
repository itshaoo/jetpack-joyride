#ifndef LEVEL1_HPP
#define LEVEL1_HPP

#include "ILevel.hpp"

class App;

class Level1 : public ILevel {
public:
    explicit Level1(App* app);
    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }
private:
    App* m_App;
    int m_TargetCoins = 150;
    bool m_Completed = false;
};

#endif // LEVEL1_HPP