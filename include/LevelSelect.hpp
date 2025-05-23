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
            return pt.x >= position.x && pt.x <= position.x + size &&
                   pt.y >= position.y && pt.y <= position.y + size;
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