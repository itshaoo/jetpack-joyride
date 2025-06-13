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

    float GetCurrentCeilingDistance() const { return m_TotalCeilingDistance; }

private:
    App* m_App;

    float m_LastWalkDistance      = 0.0f;
    float m_TotalCeilingDistance  = 0.0f;

    static constexpr float kTargetDistance = 300.0f;
    bool  m_Completed             = false;
    float m_LastDisplayDistance = 0.0f;
};

#endif // LEVEL9_HPP