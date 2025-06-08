#include "MissionDescription.hpp"
#include "config.hpp"
#include <sstream>  // for istringstream

MissionDescription::MissionDescription(int level)
  : m_Background(RESOURCE_DIR "/Image/Level/missions.png"),
    m_DescriptionText(
        RESOURCE_DIR "/font/game_font.ttf", 28, " ", Util::Color::FromName(Util::Colors::WHITE)
    ),
    m_PlayNormal(  RESOURCE_DIR "/Image/Level/play.png"),
    m_PlayPressed( RESOURCE_DIR "/Image/Level/play2.png"),
    m_playPressed(false), m_playChosen(false),
    m_ReturnNormal(  RESOURCE_DIR "/Image/Level/return.png"),
    m_ReturnPressed( RESOURCE_DIR "/Image/Level/return2.png"),
    m_returnPressed(false), m_returnChosen(false),
    m_level(level)
{
    // 任務文字列表
    static const std::vector<std::string> missions = {
        "COLLECT 150 COINS.",
        "REACH 1000M.",
        "BRUSH PAST 10 NUMBER OF RED FLASHING LIGHTS IN ONE GAME.",
        "COVER 500M ON FOOT.",
        "FLY WITHOUT TOUCHING THE GROUND FOR 500M.",
        "COLLECT 50 COINS BETWEEN 800M AND 1500M.",
        "REACH 1000M WITHOUT COLLECTING ANY COINS.",
        "COLLECT 2 VEHICLES.",
        "USE GRAVITY SUIT TO WALK ON THE CEILING FOR 300M.",
        "WEAR THE VEHICLE AND COVER 700M WITHOUT REMOVING."
    };

    // 防 out-of-range
    if (m_level < 1 || m_level > (int)missions.size()) m_level = 1;
    // 先存起來，再設定上去
    m_missionStr = missions[m_level - 1];
    m_DescriptionText.SetText(m_missionStr);
    glm::vec2 txtSz = m_DescriptionText.GetSize();
    // 固定文字起始 X 座標（例如距左邊 650px）
    const float textStartX = -(WINDOW_WIDTH * 0.5f) + 650.0f;
    m_DescPos = {
        textStartX,
        WINDOW_HEIGHT * 0.25f - txtSz.y * 0.5f
    };

    // 先取 play 按鈕尺寸
    glm::vec2 btnSz = m_PlayNormal.GetSize();
    // ====== 大小縮放設定 ======
    float playScale = 0.2f;             // 0.7 = 70%
    m_PlaySize = btnSz * playScale;
    float marginRight  = -50.0f;  // 數字越小按鈕越靠右
    float marginBottom = 80.0f; // 數字越大按鈕越靠上
    // 底左座標：右下角留 30 像素邊距
    m_PlayBL.x =  (WINDOW_WIDTH * 0.5f) - marginRight - m_PlaySize.x;
    m_PlayBL.y = -(WINDOW_HEIGHT * 0.5f) + marginBottom;

    // Return 按鈕：左上角
    glm::vec2 retSz = m_ReturnNormal.GetSize();
    float retScale = 0.8f;  // scale 可調
    m_ReturnSize = retSz * retScale;
    float retMarginLeft = 60.0f;
    float retMarginTop  = 5.0f;
    m_ReturnBL.x =-(WINDOW_WIDTH * 0.5f) + retMarginLeft;
    m_ReturnBL.y = (WINDOW_HEIGHT * 0.5f) - retMarginTop - m_ReturnSize.y;
}

void MissionDescription::Start() {
    m_playPressed = m_playChosen = false;
    m_returnPressed = m_returnChosen = false;
}

void MissionDescription::Update() {
    glm::vec2 mp = Util::Input::GetCursorPosition();
    bool down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
    bool up   = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

    if (down) {
        m_playPressed   = (mp.x>=m_PlayBL.x && mp.x<=m_PlayBL.x+m_PlaySize.x &&
                           mp.y>=m_PlayBL.y && mp.y<=m_PlayBL.y+m_PlaySize.y);
        m_returnPressed = (mp.x>=m_ReturnBL.x && mp.x<=m_ReturnBL.x+m_ReturnSize.x &&
                           mp.y>=m_ReturnBL.y && mp.y<=m_ReturnBL.y+m_ReturnSize.y);
    }
    if (up) {
        if (m_playPressed &&
            mp.x>=m_PlayBL.x && mp.x<=m_PlayBL.x+m_PlaySize.x &&
            mp.y>=m_PlayBL.y && mp.y<=m_PlayBL.y+m_PlaySize.y)
            m_playChosen = true;
        if (m_returnPressed &&
            mp.x>=m_ReturnBL.x && mp.x<=m_ReturnBL.x+m_ReturnSize.x &&
            mp.y>=m_ReturnBL.y && mp.y<=m_ReturnBL.y+m_ReturnSize.y)
            m_returnChosen = true;
        m_playPressed = m_returnPressed = false;
    }
}

void MissionDescription::Render() {
    // 背景
    Util::Transform tb;
    tb.scale = { float(WINDOW_WIDTH)/m_Background.GetSize().x,
                 float(WINDOW_HEIGHT)/m_Background.GetSize().y };
    m_Background.Draw(Util::ConvertToUniformBufferData(tb, m_Background.GetSize(), 0.0f));

    // 任務文字（只有寬度非 0 時才畫）
    glm::vec2 txtSz = m_DescriptionText.GetSize();
    if (txtSz.x > 0.0f) {
        Util::Transform dt;
        dt.translation = m_DescPos;
        m_DescriptionText.Draw(
            Util::ConvertToUniformBufferData(dt, txtSz, 0.5f)
        );
    }

    // Draw Return 按鈕
    Util::Transform rt;
    rt.translation = m_ReturnBL;
    rt.scale       = { m_ReturnSize.x / m_ReturnNormal.GetSize().x,
                       m_ReturnSize.y / m_ReturnNormal.GetSize().y };
    float zr = m_returnPressed ? 1.1f : 1.0f;
    (m_returnPressed ? m_ReturnPressed : m_ReturnNormal)
        .Draw(Util::ConvertToUniformBufferData(rt, m_ReturnNormal.GetSize(), zr));

    // Draw Play 按鈕
    Util::Transform pt;
    pt.translation = m_PlayBL;
    pt.scale       = { m_PlaySize.x / m_PlayNormal.GetSize().x,
                       m_PlaySize.y / m_PlayNormal.GetSize().y };
    float zp = m_playPressed ? 1.1f : 1.0f;
    (m_playPressed ? m_PlayPressed : m_PlayNormal)
        .Draw(Util::ConvertToUniformBufferData(pt, m_PlayNormal.GetSize(), zp));
}