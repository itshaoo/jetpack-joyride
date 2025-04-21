#include "Zapper.hpp"

Zapper::Zapper(const std::vector<std::string>& paths, const glm::vec2& startPos)
  : m_Position(startPos) {
    m_Animation = std::make_shared<Animation>(paths);
    m_Animation->SetLooping(true);
    m_Animation->SetInterval(100);
    m_Animation->Play();
    m_Animation->SetPosition(m_Position);
}

void Zapper::SetPosition(const glm::vec2& pos) {
    m_Position = pos;
    m_Animation->SetPosition(m_Position);
}

void Zapper::AddToRenderer(Util::Renderer& renderer) {
    renderer.AddChild(m_Animation);
}

void Zapper::Update(float backgroundSpeed) {
    m_Position.x -= backgroundSpeed;
    m_Animation->SetPosition(m_Position);
}

bool Zapper::IsOffScreen(int windowWidth) const {
    return m_Position.x < -(windowWidth/2) - 100.0f;
}