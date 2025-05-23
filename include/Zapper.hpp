#ifndef ZAPPER_HPP
#define ZAPPER_HPP

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Animation.hpp"
#include "Util/Renderer.hpp"
#include <Util/BGM.hpp>

class Zapper {
public:
    Zapper(const std::vector<std::string>& paths, const glm::vec2& startPos);
    void SetPosition(const glm::vec2& pos);
    void AddToRenderer(Util::Renderer& renderer);
    void Update(float backgroundSpeed);
    bool IsOffScreen(int windowWidth) const;

    glm::vec2 GetPosition() const { return m_Position; }
    glm::vec2 GetSize()     const { return m_Animation->GetScaledSize(); }
    std::shared_ptr<Animation> GetAnimationPtr() const { return m_Animation; }

private:
    glm::vec2 m_Position;
    std::shared_ptr<Animation> m_Animation;
};

#endif