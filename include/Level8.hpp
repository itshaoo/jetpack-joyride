#ifndef LEVEL8_HPP
#define LEVEL8_HPP

#include "ILevel.hpp"
class App;

class Level8 : public ILevel {
public:
    explicit Level8(App* app);
    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }
private:
    App* m_App;
    int m_EquipTarget = 2;
    bool m_Completed = false;
};

#endif // LEVEL8_HPP