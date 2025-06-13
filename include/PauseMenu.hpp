#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "ILevel.hpp"
#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Text.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <glm/glm.hpp>
#include <vector>

class PauseMenu : public ILevel {
public:
    PauseMenu(float btnSize, float spacing, const std::string &missionText, bool completed);

    void Start() override;
    void Update() override;
    void Render() override;

    bool IsResumeChosen() const { return m_resumeChosen; }
    bool IsRetryChosen() const  { return m_retryChosen; }
    bool IsQuitChosen() const   { return m_quitChosen; }

    void SetBoxImage(const std::string& path) {
        m_Box = Util::Image(RESOURCE_DIR + std::string("/") + path);
    }

private:
    Util::Image    m_Background;  // pause.png
    Util::Image    m_Box;         // complete box.png
    Util::Text     m_MissionText; // 文字說明
    glm::vec2      m_BoxPos;      // 方塊位置
    glm::vec2      m_TextPos;     // 文字位置

    struct Button {
        Util::Image normal, pressed;
        glm::vec2   pos;   // bottom-left
        glm::vec2   size;
        bool        down = false;
        Button(const std::string &n, const std::string &p, const glm::vec2 &bl, const glm::vec2 &sz)
          : normal(n), pressed(p), pos(bl), size(sz) {}
        bool contains(const glm::vec2 &pt) const {
            const glm::vec2 correction(75.0f, 25.0f);
            glm::vec2 correctedPos = pos - correction;
            glm::vec2 effectiveSize = size - glm::vec2(3.0f, 0.0f);
            return pt.x >= correctedPos.x && pt.x <= correctedPos.x + effectiveSize.x &&
                   pt.y >= correctedPos.y && pt.y <= correctedPos.y + effectiveSize.y;
        }
        void draw(bool isDown) {
            Util::Transform t;
            t.translation = pos;
            t.scale = { size.x/normal.GetSize().x, size.y/normal.GetSize().y };
            float z = isDown ? 1.1f : 1.0f;
            (isDown ? pressed : normal)
                .Draw(Util::ConvertToUniformBufferData(t, normal.GetSize(), z));
        }
    };

    std::vector<Button> m_buttons; // order: quit, retry, resume
    bool m_resumeChosen = false;
    bool m_retryChosen  = false;
    bool m_quitChosen   = false;
};

#endif