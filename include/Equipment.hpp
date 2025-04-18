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
    void Update(float backgroundSpeed); // 接受背景速度作為參數
    bool IsOffScreen() const;

    static void UpdateEquipments(
        float spawnInterval,
        std::vector<std::shared_ptr<Equipment>>& equipments,
        Util::Renderer& renderer,
        float backgroundSpeed);

private:
    glm::vec2 m_Position = {650.0f, 0.0f};
    std::shared_ptr<Animation> equipmentAnimation;

    static float equipmentSpawnTimer;
};