#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <Animation.hpp>
#include <Util/Renderer.hpp>

class Equipment {
public:
    Equipment();

    void SetPosition(const glm::vec2& position);
    void AddToRenderer(Util::Renderer& renderer);
    void Update(float backgroundSpeed);
    bool IsOffScreen() const;
    std::shared_ptr<Animation> equipmentAnimation;

    static void UpdateEquipments(
        float spawnInterval,
        std::vector<std::shared_ptr<Equipment>>& equipments,
        Util::Renderer& renderer,
        float backgroundSpeed);

    glm::vec2 GetPosition() const { return m_Position; }
    glm::vec2 GetSize()     const { return equipmentAnimation->GetScaledSize(); }

    void Render();

    static void ResetSpawnTimer() {
        equipmentSpawnTimer = 0.0f;
    }

    std::shared_ptr<Animation> GetAnimation() const {
        return equipmentAnimation;
    }

private:
    glm::vec2 m_Position = {650.0f, 0.0f};

    static float equipmentSpawnTimer;
};