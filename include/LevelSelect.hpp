#ifndef LEVELSELECT_HPP
#define LEVELSELECT_HPP

#include "ILevel.hpp"
#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Text.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class LevelSelect : public ILevel {
public:
    struct Button {
        Util::Image normal, pressed;
        Util::Text  label;
        glm::vec2   position;
        float       size;
        bool        isPressed = false;

        Button(const std::string &nPath,
               const std::string &pPath,
               const std::string &fontPath,
               int fontSize,
               const std::string &text,
               const glm::vec2 &bl,
               float s)
          : normal(nPath), pressed(pPath), label(
                fontPath, fontSize, text,
                Util::Color::FromName(Util::Colors::WHITE)
            ), position(bl), size(s) {}

        bool Contains(const glm::vec2 &pt) const {
            const glm::vec2 correction = glm::vec2(50.0f, 50.0f);
            glm::vec2 correctedPosition = position - correction;
            // 設定有效區域的寬度與高度，減去右邊與上邊額外的 3 像素
            float effectiveWidth = size - 3.0f;
            float effectiveHeight = size - 3.0f;

            return pt.x >= correctedPosition.x &&
                   pt.x <= (correctedPosition.x + effectiveWidth) &&
                   pt.y >= correctedPosition.y &&
                   pt.y <= (correctedPosition.y + effectiveHeight);
        }
    };

    LevelSelect(float btnSize, float spacing);
    void Start() override;
    void Update() override;
    void Render() override;
    bool IsLevelChosen() const { return chosen; }
    int  GetChosenLevel() const { return chosenLevel; }

private:
    std::vector<Button> buttons;
    int m_pressedIndex = -1;  // -1 = none, 0-9 = pressed button index
    bool chosen = false;
    int  chosenLevel = -1;
};

#endif