#ifndef ZAPPER_HPP
#define ZAPPER_HPP

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Animation.hpp"
#include "Util/Renderer.hpp"

class Zapper {
public:
    Zapper(const std::vector<std::string>& paths, const glm::vec2& startPos);
    void SetPosition(const glm::vec2& pos);
    void AddToRenderer(Util::Renderer& renderer);
    void Update(float backgroundSpeed);
    bool IsOffScreen(int windowWidth) const;

private:
    glm::vec2 m_Position;
    std::shared_ptr<Animation> m_Animation;
};

#endif