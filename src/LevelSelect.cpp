#include "LevelSelect.hpp"
#include "config.hpp"
#include <iostream>

LevelSelect::LevelSelect(float btnSize, float spacing) {
    const std::string base = RESOURCE_DIR "/Image/Level";
    float colSpacing = spacing + 80;
    float rowSpacing = 80.0f;

    // 計算置中用左下角原點
    float totalWidth  = 5 * btnSize + 4 * colSpacing;
    float totalHeight = 2 * btnSize +     rowSpacing;
    glm::vec2 originBL = { -totalWidth*0.5f, -totalHeight*0.5f };

    // 可調整整體位置偏移
    float offsetX = 50.0f;
    float offsetY = -5.0f;
    originBL.x += offsetX;
    originBL.y += offsetY;

    for (int i = 0; i < 10; ++i) {
        int row = i / 5;   // 0 = top, 1 = bottom
        int col = i % 5;
        glm::vec2 bl = {
            originBL.x + col * (btnSize + colSpacing),
            originBL.y + (1 - row) * (btnSize + rowSpacing)
        };
        buttons.emplace_back(
            base + "/button1.png",
            base + "/button2.png",
            RESOURCE_DIR "/font/game_font.ttf",
            24,
            std::to_string(i + 1),
            bl,
            btnSize
        );
    }
}

void LevelSelect::Start() {
    chosen = false;
    m_pressedIndex = -1;
}

void LevelSelect::Update() {
    glm::vec2 mousePt = Util::Input::GetCursorPosition();
    bool down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
    bool up   = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

    if (down) {
        // 按下滑鼠時，記錄按到哪個按鈕
        for (auto &btn : buttons) {
            btn.isPressed = btn.Contains(mousePt);
        }
    }
    if (up) {
        // 放開時，只在原本 isPressed 的按鈕中，再次確認游標仍在同按鈕就選擇
        for (int i = 0; i < (int)buttons.size(); ++i) {
            if (buttons[i].isPressed && buttons[i].Contains(mousePt)) {
                chosen = true;
                chosenLevel = i + 1;    // 記錄第 i+1 關
                break;
            }
        }
        // 清空所有 isPressed
        for (auto &btn : buttons) {
            btn.isPressed = false;
        }
    }
}

void LevelSelect::Render() {
    // 背景 (z=0)
    Util::Image bg(RESOURCE_DIR "/Image/Level/bg.png");
    Util::Transform tb;
    tb.scale = { float(WINDOW_WIDTH)/bg.GetSize().x,
                 float(WINDOW_HEIGHT)/bg.GetSize().y };
    bg.Draw(Util::ConvertToUniformBufferData(tb, bg.GetSize(), 0.0f));

    const float textOffsetX = -28.0f;
    const float textOffsetY = -33.0f;

    for (int i = 0; i < (int)buttons.size(); ++i) {
        auto &btn = buttons[i];

        // 按鈕圖示 (z=1)
        Util::Transform bt;
        bt.translation = btn.position;
        bt.scale = { btn.size/btn.normal.GetSize().x,
                     btn.size/btn.normal.GetSize().y };
        Core::Matrices bm = Util::ConvertToUniformBufferData(bt, btn.normal.GetSize(), 1.0f);
        if (btn.isPressed) {
            btn.pressed.Draw(bm);
        } else {
            btn.normal.Draw(bm);
        }

        // 畫文字 (z=1.2)
        glm::vec2 textSize = btn.label.GetSize();
        glm::vec2 textPos = {
            btn.position.x + (btn.size - textSize.x)*0.5f + textOffsetX,
            btn.position.y + (btn.size - textSize.y)*0.5f + textOffsetY
        };
        Util::Transform lt;
        lt.translation = textPos;
        btn.label.Draw(Util::ConvertToUniformBufferData(lt, textSize, 1.2f));
    }
}