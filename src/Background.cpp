#include "Background.hpp"

Background::Background()
    : m_Phase(Phase::INITIAL),
      m_ScrollX(0.0f),
      m_ScaledWidth(static_cast<float>(WINDOW_WIDTH))
{
    // --- 初始化 Initial 阶段使用的图像 ---
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Home/start.png");
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab1.png");
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab2.png");

    // --- 初始化 Looping 阶段使用的循环背景 ---
    m_LoopingImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab1.png");
    m_LoopingImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab2.png");

    // --- 初始化 Home 界面上覆盖的叠加图（启动画面用） ---
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/board.png");
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/table.png");
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/llamp.png");
    m_StartOverlays.emplace_back(RESOURCE_DIR "/Image/Home/red light.png");

    // --- 初始化“播放 Rec 板”动画 ---
    recBoardAnim.intervalMs = 200.0f;
    for (int i = 1; i <= 4; ++i) {
        recBoardAnim.frames.emplace_back(
            RESOURCE_DIR + std::string("/Image/Home/Rec board/rec board") + std::to_string(i) + ".png"
        );
    }

    // --- 初始化“播放 Wall 板”动画 ---
    wallBoardAnim.intervalMs = 200.0f;
    for (int i = 1; i <= 4; ++i) {
        wallBoardAnim.frames.emplace_back(
            RESOURCE_DIR + std::string("/Image/Home/Wall board/w board") + std::to_string(i) + ".png"
        );
    }

    // --- 初始化“Home 界面红灯”动画（light1.png~light6.png）---
    redLightAnim.intervalMs = 150.0f;
    for (int i = 1; i <= 6; ++i) {
        redLightAnim.frames.emplace_back(
            RESOURCE_DIR "/Image/Home/Red light/light" + std::to_string(i) + ".png"
        );
    }

    // --- 初始化 LOOPING 阶段的“循环红灯”列表，仅示例 1 个，如果需要多个可以 push 更多  ---
    std::vector<glm::vec2> redOffsets = {
        { 450.0f, 190.0f }  // 该红灯在 tile 左下角的偏移（相对坐标）
    };
    for (auto &ofs : redOffsets) {
        LoopRedLight lr;
        lr.anim.intervalMs = 150.0f;
        // 载入循环红灯的静态贴图（Lab 目录下的 red light.png）
        for (int i = 1; i <= 4; ++i) {
            lr.anim.frames.emplace_back(
                RESOURCE_DIR "/Image/Background/Lab/red light.png"
            );
        }
        lr.offset = ofs;
        m_LoopRedLights.push_back(std::move(lr));
    }
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
    m_ScrollX -= backgroundSpeed;  // 使用动态背景速度

    // “滴答”更新 Initial 阶段相关动画
    auto tick = [&](OverlayAnim &anim) {
        anim.timer += Util::Time::GetDeltaTimeMs();
        if (anim.timer >= anim.intervalMs) {
            anim.timer -= anim.intervalMs;
            anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
        }
    };
    tick(recBoardAnim);
    tick(wallBoardAnim);

    // Home 界面的红灯动画：只有在收到 NotifyLogoOffScreen 后才开始播放
    if (m_RedLightActive) {
        tick(redLightAnim);
    }

    // 当整个 Initial 背景滑出画面左侧，切换到 LOOPING
    if (m_ScrollX <= -m_ScaledWidth) {
        m_Phase = Phase::LOOPING;
        m_ScrollX = 0.0f;
        // 进入 LOOPING 时，可将所有 LoopRedLight 的动画重置
        for (auto &lr : m_LoopRedLights) {
            lr.anim.timer = 0.0f;
            lr.anim.currentFrame = 0;
        }
    }
}

void Background::renderInitial() {
    for (size_t i = 0; i < m_InitialImages.size(); ++i) {
        float posX = m_ScrollX + i * m_ScaledWidth;
        auto &img = m_InitialImages[i];

        // 计算缩放比例，使图片充满窗口
        float originalWidth  = img.GetSize().x;
        float originalHeight = img.GetSize().y;
        float scaleX = static_cast<float>(WINDOW_WIDTH)  / originalWidth;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / originalHeight;

        Util::Transform transform;
        transform.translation = glm::vec2(posX, 0.0f);
        transform.scale       = glm::vec2(scaleX, scaleY);
        auto matrices = Util::ConvertToUniformBufferData(transform, img.GetSize(), 0.0f);
        img.Draw(matrices);

        // 只在 i==0 (即 start.png 这一帧) 上绘制 Home 界面的叠加
        if (i == 0) {
            constexpr float overlayScales[5] = { 0.7f, 0.7f, 0.7f, 0.6f };
            constexpr glm::vec2 overlayOffsets[5] = {
                {-540, -260},
                {-460, -170},
                {-410, 190},
                {250, 150}
            };
            for (size_t idx = 0; idx < m_StartOverlays.size(); ++idx) {
                auto &ov = m_StartOverlays[idx];
                Util::Transform ot = transform;
                ot.scale       *= overlayScales[idx];
                ot.translation += overlayOffsets[idx];
                ov.Draw(Util::ConvertToUniformBufferData(ot, ov.GetSize(), 0.1f));
            }

            // 绘制 Rec 板动画
            {
                auto &imgRec = recBoardAnim.frames[recBoardAnim.currentFrame];
                Util::Transform ot = transform;
                ot.scale       *= 0.6f;
                ot.translation += glm::vec2(300.0f, -170.0f);
                imgRec.Draw(Util::ConvertToUniformBufferData(ot, imgRec.GetSize(), 0.15f));
            }

            // 绘制 Wall 板动画
            {
                auto &imgWall = wallBoardAnim.frames[wallBoardAnim.currentFrame];
                Util::Transform ot = transform;
                ot.scale       *= 0.6f;
                ot.translation += glm::vec2(-70.0f, 270.0f);
                imgWall.Draw(Util::ConvertToUniformBufferData(ot, imgWall.GetSize(), 0.15f));
            }

            // Home 界面上的红灯动画
            if (m_RedLightActive) {
                auto &imgRL = redLightAnim.frames[redLightAnim.currentFrame];
                Util::Transform ot = transform;
                ot.scale       *= 0.5f;
                ot.translation += glm::vec2(210.0f, 115.7f);
                imgRL.Draw(Util::ConvertToUniformBufferData(ot, imgRL.GetSize(), 0.15f));
            }
        }
    }
}

void Background::updateLooping() {
    // 背景滚动
    m_ScrollX -= backgroundSpeed;
    if (m_ScrollX <= -m_ScaledWidth) {
        m_ScrollX += m_ScaledWidth;
    }

    // “滴答”更新所有“循环红灯”静态帧
    float dt = Util::Time::GetDeltaTimeMs();
    for (auto &lr : m_LoopRedLights) {
        lr.anim.timer += dt;
        if (lr.anim.timer >= lr.anim.intervalMs) {
            lr.anim.timer -= lr.anim.intervalMs;
            lr.anim.currentFrame = (lr.anim.currentFrame + 1) % lr.anim.frames.size();
        }
    }

    // 同时更新 Home/Red light 的循环动画帧
    if (m_RedLightActive) {
        redLightAnim.timer += dt;
        if (redLightAnim.timer >= redLightAnim.intervalMs) {
            redLightAnim.timer -= redLightAnim.intervalMs;
            redLightAnim.currentFrame = (redLightAnim.currentFrame + 1) % redLightAnim.frames.size();
        }
    }

    // 让“循环红灯”跟随背景一起往左移动，当完全滑出左侧时，回到最右侧
    for (auto &lr : m_LoopRedLights) {
        // worldX 表示红灯在世界坐标中的 X，初始化时我们把它看作 posX + offset.x
        // 这里我们只用 posX + offset.x 计算方式即可
        // 不需要单独保存 worldX，直接使用 posX 叠加 offset 即可
        // 所以这里不用额外更新 lr.worldX
    }
}

void Background::renderLooping() {
    // 先画三格循环背景
    for (int i = 0; i < 3; ++i) {
        float posX = m_ScrollX + i * m_ScaledWidth;
        auto &image = m_LoopingImages[i % m_LoopingImages.size()];

        float w = image.GetSize().x;
        float h = image.GetSize().y;
        float scaleX = float(WINDOW_WIDTH)  / w;
        float scaleY = float(WINDOW_HEIGHT) / h;

        Util::Transform bgT;
        bgT.translation = { posX, 0.0f };
        bgT.scale       = { scaleX, scaleY };
        image.Draw(Util::ConvertToUniformBufferData(bgT, image.GetSize(), 0.0f));
    }

    // 然后在每个“循环红灯”上方，绘制静态红灯 + 叠加动画
    constexpr float redScale = 0.7f;     // 红灯图的缩放比例
    constexpr float animScale = 0.7f;    // 叠加动画的缩放比例
    for (int i = 0; i < 3; ++i) {
        float posX = m_ScrollX + i * m_ScaledWidth;
        for (auto &lr : m_LoopRedLights) {
            // 计算静态红灯在这一格的位置：tile 左下角 posX + lr.offset.x
            float lightX = posX + lr.offset.x;
            float lightY = lr.offset.y;

            // 1) 绘制红灯静态帧
            auto &staticFrame = lr.anim.frames[lr.anim.currentFrame];
            Util::Transform rt;
            rt.translation = { lightX, lightY };
            rt.scale       = { redScale, redScale };
            staticFrame.Draw(Util::ConvertToUniformBufferData(rt, staticFrame.GetSize(), 0.1f));

            // 2) 绘制覆盖在红灯上的动画帧（来自 Home/Red light/light1~light6）
            if (m_RedLightActive) {
                auto &animFrame = redLightAnim.frames[redLightAnim.currentFrame];
                Util::Transform at;
                at.translation = { lightX + 30.0f, lightY - 27.0f };
                at.scale       = { animScale, animScale };
                animFrame.Draw(Util::ConvertToUniformBufferData(at, animFrame.GetSize(), 0.2f));
            }
        }
    }
}

// -------- 新增：回傳目前畫面上所有紅燈的 AABB，供 Level4 做碰撞檢測 --------
std::vector<std::pair<glm::vec2, glm::vec2>> Background::GetRedLightBounds() const {
    std::vector<std::pair<glm::vec2, glm::vec2>> result;

    // 只有在 LOOPING 階段才會有「循環紅燈」
    if (m_Phase != Phase::LOOPING) {
        return result;
    }
    // 取得每個 LoopRedLight 在背景 Tile 編號 i(0~2) 的世界座標
    // 複製 renderLooping() 中的邏輯：有三個 tile (i=0,1,2)，其 posX = m_ScrollX + i*m_ScaledWidth
    for (int i = 0; i < 3; ++i) {
        float baseTileX = m_ScrollX + i * m_ScaledWidth;
        for (const auto& lr : m_LoopRedLights) {
            // 紅燈左下角 world 座標
            float worldX = baseTileX + lr.offset.x;
            float worldY = lr.offset.y;
            // 取得紅燈影像的原始大小
            // 由於 OverlayAnim.anim.frames 存放的正是紅燈的每一個靜態幀
            const Util::Image& sampleImg = lr.anim.frames[lr.anim.currentFrame];
            auto origSize = sampleImg.GetSize(); // glm::vec2(width, height)

            // 縮放後尺寸 = 原始尺寸 * kRedScale
            glm::vec2 scaledSize = { origSize.x * kRedScale, origSize.y * kRedScale };
            // 左下角位置需考慮「縮放錨點」：Background 在 renderLooping 裡是直接用 (lightX, lightY) 當左下
            // 所以此處也直接用 (worldX, worldY) 當作左下。
            glm::vec2 pos = { worldX, worldY };
            result.emplace_back(std::make_pair(pos, scaledSize));
        }
    }
    return result;
}

std::pair<glm::vec2, glm::vec2> Background::GetInitialRedLightBounds() const {
    // 只有 INITIAL 階段上才會有這張紅燈
    if (m_Phase != Phase::INITIAL) {
        return { {0.0f, 0.0f}, {0.0f, 0.0f} };
    }

    // 1) 先拿到 start.png（m_InitialImages[0]）的尺寸
    const Util::Image& startImg = m_InitialImages[0];
    auto origSize = startImg.GetSize(); // (width, height)

    // 2) start.png 被縮放到整個窗口
    float scaleX = static_cast<float>(WINDOW_WIDTH)  / origSize.x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / origSize.y;

    // 3) 初始畫面上，紅燈是 m_StartOverlays[3]
    //    在 renderInitial() 裡，它的 offset 定義為 overlayOffsets[3]
    //    overlayOffsets[3] = {250.0f, 150.0f}（相對 start.png 左下角）。
    //
    //    而它的 scale = start 的 scale * overlayScales[3]，
    //    overlayScales[3] 在 renderInitial() 是 0.6。
    float overlayScale = 0.6f;
    glm::vec2 overlayOffset = {250.0f, 150.0f};

    // 4) 加上當前背景位移 m_ScrollX，才是正確的世界座標
    float posX = m_ScrollX /* ＝ transform.translation.x */;
    glm::vec2 worldPos = { posX + overlayOffset.x, overlayOffset.y };

    // 5) 計算紅燈原始尺寸
    const Util::Image& redImg = m_StartOverlays[3];
    auto redOrigSize = redImg.GetSize(); // (width, height)

    // 6) 紅燈實際繪製尺寸 = redOrigSize * overlayScale * (後續 X/Y 再乘 scaleX/scaleY)
    //    先算「紅燈在原始 start.png 座標下，放大 overlayScale 後」的尺寸：
    glm::vec2 scaledOverlay = { redOrigSize.x * overlayScale, redOrigSize.y * overlayScale };
    //    最後畫到畫面上時，整張 start.png 又被縮放了 scaleX、scaleY，因此：
    glm::vec2 worldSize = { scaledOverlay.x * scaleX, scaledOverlay.y * scaleY };
    return { worldPos, worldSize };
}