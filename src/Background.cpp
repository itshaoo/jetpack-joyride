#include "Background.hpp"

Background::Background()
    : m_Phase(Phase::INITIAL), m_ScrollX(0.0f)
{
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Home/start.png");
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab1.png");
    m_InitialImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab2.png");

    m_LoopingImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab1.png");
    m_LoopingImages.emplace_back(RESOURCE_DIR "/Image/Background/Lab/lab2.png");

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
    m_ScrollX -= 4.0f;

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
    }
}

void Background::updateLooping() {
    m_ScrollX -= 4.0f;
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