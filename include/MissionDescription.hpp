#ifndef MISSIONDESCRIPTION_HPP
#define MISSIONDESCRIPTION_HPP

#include "ILevel.hpp"
#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Text.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <glm/glm.hpp>
#include "config.hpp"
#include <vector>
#include <string>

class MissionDescription : public ILevel {
public:
    explicit MissionDescription(int level);

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsPlayChosen() const { return m_playChosen; }
    bool IsReturnChosen() const { return m_returnChosen; }

    const std::string& GetMissionString() const { return m_missionStr; }

    int GetLevel() const { return m_level; }

private:
    Util::Image m_Background;

    Util::Text  m_DescriptionText;
    glm::vec2   m_DescPos;

    std::string m_missionStr;

    int m_level;

    // Play 按鈕
    Util::Image m_PlayNormal;      // play.png
    Util::Image m_PlayPressed;     // play2.png
    glm::vec2   m_PlayBL;          // play 按鈕底左座標
    glm::vec2   m_PlaySize;        // play 按鈕大小
    bool        m_playPressed;     // 滑鼠按住時
    bool        m_playChosen;      // 按鈕釋放後確認進遊戲

    // Return 按鈕
    Util::Image m_ReturnNormal;
    Util::Image m_ReturnPressed;
    glm::vec2   m_ReturnBL;
    glm::vec2   m_ReturnSize;
    bool        m_returnPressed;
    bool        m_returnChosen;
};

#endif // MISSIONDESCRIPTION_HPP