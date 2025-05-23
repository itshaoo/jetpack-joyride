#include "Coin.hpp"
#include <iostream>
Coin::Coin(const std::vector<std::string>& frames, const glm::vec2& startPos)
  : m_Position(startPos) {
    m_Animation = std::make_shared<Animation>(frames, /*z=*/3.0f);
    m_Animation->SetLooping(true);
    m_Animation->SetInterval(100);
    m_Animation->Play();
    m_Animation->SetPosition(m_Position);
}

void Coin::SetPosition(const glm::vec2& pos) {
    m_Position = pos;
    m_Animation->SetPosition(m_Position);
}

void Coin::AddToRenderer(Util::Renderer& renderer) {
    renderer.AddChild(m_Animation);
}

void Coin::Update(float backgroundSpeed) {
    m_Position.x -= backgroundSpeed;
    m_Animation->SetPosition(m_Position);
}

bool Coin::IsOffScreen(int windowWidth) const {
    return m_Position.x < -(windowWidth/2) - 100.0f;
}