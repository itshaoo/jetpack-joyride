#ifndef LEVEL3_HPP
#define LEVEL3_HPP

#include "ILevel.hpp"
#include <vector>
#include "Util/Text.hpp"
#include <glm/vec2.hpp>
#include <memory>

class App;

class Level3 : public ILevel {
public:
    explicit Level3(App* app);
    ~Level3() override = default;

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsCompleted() const override { return m_Completed; }

    // 新增：对外暴露红灯已触碰计数
    int GetRedTouchedCount() const { return m_RedTouchedCount; }

    int   m_RedTouchedCount = 0;
private:
    App* m_App;

    int  m_TargetRedLight   = 10;
    std::vector<bool> m_PrevCollided;

    bool  m_Completed      = false;

    bool m_PrevInitialCollided = false;
};

#endif // LEVEL3_HPP