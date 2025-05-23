#include "Background.hpp"

Background::Background()
    : m_Phase(Phase::INITIAL), m_ScrollX(0.0f)
{
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Home/start.png");
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab1.png");
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab2.png");

    m_LoopingImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab1.png");
    m_LoopingImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab2.png");

    // 載入你放在 Image/Home 裡的五張圖
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/board.png");
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/table.png");
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/llamp.png");
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/red light.png");

    // Rec board
    recBoardAnim.intervalMs = 200.0f;
    for (int i = 1; i <= 4; ++i)
        recBoardAnim.frames.emplace_back(
          RESOURCE_DIR + std::string("/Image/Home/Rec board/rec board") + std::to_string(i) + ".png"
        );

    // Wall board
    wallBoardAnim.intervalMs = 200.0f;
    for (int i = 1; i <= 4; ++i)
        wallBoardAnim.frames.emplace_back(
          RESOURCE_DIR + std::string("/Image/Home/Wall board/w board") + std::to_string(i) + ".png"
        );

    // Red light 載入但暫不啟動
    redLightAnim.intervalMs = 150.0f;
    for (int i = 1; i <= 4; ++i)
        redLightAnim.frames.emplace_back(
          RESOURCE_DIR "/Image/Home/Red light/light" + std::to_string(i) + ".png"
        );

    m_ScaledWidth = static_cast<float>(WINDOW_WIDTH);
}

void Background::Update() {
    switch (m_Phase) {
        case Phase::INITIAL:
            updateInitial();
            break;
        case Phase::LOOPING:
            updateLooping();
            break;
    }
}

void Background::Render() {
    switch (m_Phase) {
        case Phase::INITIAL:
            renderInitial();
            break;
        case Phase::LOOPING:
            renderLooping();
            break;
    }
}

void Background::updateInitial() {
    m_ScrollX -= backgroundSpeed;  // 使用動態背景速度

    // 共用的計時器函式
    auto tick = [&](OverlayAnim &anim) {
        anim.timer += Util::Time::GetDeltaTimeMs();
        if (anim.timer >= anim.intervalMs) {
            anim.timer -= anim.intervalMs;
            anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
        }
    };
    // Rec 與 Wall 一開始就跑
    tick(recBoardAnim);
    tick(wallBoardAnim);

    // Red light 只在 Flag 起才跑
    if (m_RedLightActive)
        tick(redLightAnim);

    if (m_ScrollX <= -m_ScaledWidth) {
        m_Phase = Phase::LOOPING;
        m_ScrollX = 0.0f;
    }
}

void Background::renderInitial() {
    for (size_t i = 0; i < m_InitialImages.size(); ++i) {
        float posX = m_ScrollX + i * m_ScaledWidth;

        float originalWidth = m_InitialImages[i].GetSize().x;
        float originalHeight = m_InitialImages[i].GetSize().y;

        float scaleX = static_cast<float>(WINDOW_WIDTH) / originalWidth;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / originalHeight;

        Util::Transform transform;
        transform.translation = glm::vec2(posX, 0.0f);
        transform.scale = glm::vec2(scaleX, scaleY);

        Core::Matrices matrices = Util::ConvertToUniformBufferData(transform, m_InitialImages[i].GetSize(), 0.0f);
        m_InitialImages[i].Draw(matrices);

        constexpr float overlayScales[5] = { 0.7f, 0.7f, 0.7f, 0.6f };
        constexpr glm::vec2 overlayOffsets[5] = {
            {-540, -260 },
            {-460,-170 },
            {-410, 190 },
            {250,150 }
        };

        if (i == 0) {
            for (size_t idx = 0; idx < m_StartOverlays.size(); ++idx) {
                auto& ov = m_StartOverlays[idx];
                Util::Transform ot = transform;
                ot.scale       *= overlayScales[idx];
                ot.translation += overlayOffsets[idx];
                ov.Draw(Util::ConvertToUniformBufferData(ot, ov.GetSize(), 0.1f));
            }

            // Rec board
            {
                auto &img = recBoardAnim.frames[recBoardAnim.currentFrame];
                Util::Transform ot = transform;
                ot.scale       *= 0.6f;
                ot.translation += glm::vec2(300.0f, -170.0f);
                img.Draw(Util::ConvertToUniformBufferData(ot, img.GetSize(), 0.15f));
            }

            // Wall board
            {
                auto &img = wallBoardAnim.frames[wallBoardAnim.currentFrame];
                Util::Transform ot = transform;
                ot.scale       *= 0.6f;
                ot.translation += glm::vec2(-70.0f, 270.0f);
                img.Draw(Util::ConvertToUniformBufferData(ot, img.GetSize(), 0.15f));
            }

            // Red light (only if activated)
            if (m_RedLightActive) {
                auto &img = redLightAnim.frames[redLightAnim.currentFrame];
                Util::Transform ot = transform;
                ot.scale       *= 0.5f;
                ot.translation += glm::vec2(210.0f, 115.7f);
                img.Draw(Util::ConvertToUniformBufferData(ot, img.GetSize(), 0.15f));
            }
        }
    }
}

void Background::updateLooping() {
    m_ScrollX -= backgroundSpeed;
    if (m_ScrollX <= -m_ScaledWidth) {
        m_ScrollX += m_ScaledWidth;
    }
}

void Background::renderLooping() {

    for (int i = 0; i < 3; ++i) {
        float posX = m_ScrollX + i * m_ScaledWidth;

        auto &image = m_LoopingImages[i % m_LoopingImages.size()];

        float originalWidth = image.GetSize().x;
        float originalHeight = image.GetSize().y;

        float scaleX = static_cast<float>(WINDOW_WIDTH) / originalWidth;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / originalHeight;

        Util::Transform transform;
        transform.translation = glm::vec2(posX, 0.0f);
        transform.scale = glm::vec2(scaleX, scaleY);

        Core::Matrices matrices = Util::ConvertToUniformBufferData(transform, image.GetSize(), 0.0f);
        image.Draw(matrices);
    }
}