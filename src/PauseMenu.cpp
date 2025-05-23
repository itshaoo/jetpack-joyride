#include "PauseMenu.hpp"
#include "config.hpp"

PauseMenu::PauseMenu(float btnSize,
                     float spacing,
                     const std::string &missionText,
                     bool completed)
  : m_Background(RESOURCE_DIR "/Image/Pause/pause.png"),
    // 如果完成，就用 complete box2.png，否則用 complete box.png
    m_Box(
       RESOURCE_DIR "/Image/Pause/"
       + std::string(completed ? "complete box2.png" : "complete box.png")
    ),
    m_MissionText(
        RESOURCE_DIR "/font/game_font.ttf",
        24,
        missionText,
        Util::Color::FromName(Util::Colors::WHITE)
    )
{

    // ==== 1) 算方塊與文字位置 ====
    // 放在畫面的正中央偏上
    glm::vec2 boxSz = m_Box.GetSize();
    float scaleBox     = 0.25f;     // 縮到原來的 25%
    float shiftX       = 450.0f;    // 向左額外移動 450px
    m_BoxPos = {
        -boxSz.x * scaleBox * 0.5f - shiftX,  // 先做置中再往左多移動 shiftX
        WINDOW_HEIGHT * 0.5f * 0.2f // 屏幕上方 20%
    };

    // 文字放在方塊右側
    glm::vec2 txtSz = m_MissionText.GetSize();
    m_TextPos = {
        m_BoxPos.x + 500.0f,//boxSz.x * scaleBox + 20.0f,
        m_BoxPos.y + (boxSz.y * scaleBox - txtSz.y) * 0.5f
    };

    // ==== 2) 按鈕區 ====
    float hSpacing     = spacing + 100;     // 按鈕間距
    float bottomMargin = 60.0f;             // 距底邊
    float xOffset      = 70.0f;             // 水平偏移
    float yOffset      = 10.0f;             // 垂直偏移

    // 按鈕長寬比維持原圖
    glm::vec2 imgSz = Util::Image(RESOURCE_DIR "/Image/Pause/quit.png").GetSize();
    float scale     = btnSize / imgSz.x;
    glm::vec2 btnSz = imgSz * scale;

    // 3) 水平起點 (置中再加偏移)
    float totalW = 3*btnSz.x + 2*hSpacing;
    float startX = -totalW*0.5f + xOffset;

    // 4) 垂直位置 (靠底部再加偏移)
    float y = -(WINDOW_HEIGHT*0.5f) + bottomMargin + yOffset;

    // 5) 建三個按鈕
    m_buttons.emplace_back(
        std::string(RESOURCE_DIR) + "/Image/Pause/quit.png",
        std::string(RESOURCE_DIR) + "/Image/Pause/quit2.png",
        glm::vec2{startX, y}, btnSz);
    m_buttons.emplace_back(
        std::string(RESOURCE_DIR) + "/Image/Pause/retry.png",
        std::string(RESOURCE_DIR) + "/Image/Pause/retry2.png",
        glm::vec2{startX + btnSz.x + hSpacing, y}, btnSz);
    m_buttons.emplace_back(
        std::string(RESOURCE_DIR) + "/Image/Pause/resume.png",
        std::string(RESOURCE_DIR) + "/Image/Pause/resume2.png",
        glm::vec2{startX + 2*(btnSz.x + hSpacing), y}, btnSz);
}

void PauseMenu::Start() {
    m_resumeChosen = m_retryChosen = m_quitChosen = false;
    for (auto &b : m_buttons) b.down = false;
}

void PauseMenu::Update() {
    glm::vec2 mp = Util::Input::GetCursorPosition();
    bool down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
    bool up   = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

    if (down) {
        for (auto &b : m_buttons) {
            b.down = b.contains(mp);
        }
    }
    if (up) {
        if (m_buttons[2].down && m_buttons[2].contains(mp)) m_resumeChosen = true;
        if (m_buttons[1].down && m_buttons[1].contains(mp)) m_retryChosen  = true;
        if (m_buttons[0].down && m_buttons[0].contains(mp)) m_quitChosen   = true;
        for (auto &b : m_buttons) b.down = false;
    }
}

void PauseMenu::Render() {
    // 1) 背景 (z = 1.0f)
    Util::Transform tb;
    tb.scale = { float(WINDOW_WIDTH)  / m_Background.GetSize().x,
                 float(WINDOW_HEIGHT) / m_Background.GetSize().y };
    m_Background.Draw(Util::ConvertToUniformBufferData(
        tb, m_Background.GetSize(), 1.0f));

    // 2) 中間方塊 (z = 1.1f)
    Util::Transform bt;
    bt.translation = m_BoxPos;
    float boxScale = 0.5f;
    bt.scale = { boxScale, boxScale };
    m_Box.Draw(Util::ConvertToUniformBufferData(
        bt, m_Box.GetSize(), 1.1f));

    // 3) 任務文字 (z = 1.2f)
    Util::Transform tt;
    tt.translation = m_TextPos;
    m_MissionText.Draw(Util::ConvertToUniformBufferData(
        tt, m_MissionText.GetSize(), 1.2f));

    // 4) 畫按鈕 (normal: z=1.3f, pressed: z=1.4f)
    for (auto &b : m_buttons) {
        Util::Transform t;
        t.translation = b.pos;
        t.scale = { b.size.x / b.normal.GetSize().x,
                    b.size.y / b.normal.GetSize().y };
        bool isDown = b.down;
        float z = isDown ? 1.4f : 1.3f;
        (isDown ? b.pressed : b.normal)
            .Draw(Util::ConvertToUniformBufferData(t, b.normal.GetSize(), z));
    }
}