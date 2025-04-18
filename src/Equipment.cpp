#include "Equipment.hpp"
#include <cstdlib>
#include <iostream>
#include <Util/Time.hpp>

float Equipment::equipmentSpawnTimer = 0.0f;

Equipment::Equipment() {
    std::vector<std::string> equipmentFrames = {
        RESOURCE_DIR "/Image/PowerUp/powerUp0.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp1.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp2.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp3.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp4.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp5.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp6.png",
        RESOURCE_DIR "/Image/PowerUp/powerUp7.png"
    };

    equipmentAnimation = std::make_shared<Animation>(equipmentFrames);
    equipmentAnimation->SetLooping(true);
    equipmentAnimation->SetInterval(100);
    equipmentAnimation->SetVisible(true);
    equipmentAnimation->Play();
    equipmentAnimation->SetWorldObject(false);
}

void Equipment::SetPosition(const glm::vec2& position) {
    m_Position = position;
    equipmentAnimation->SetPosition(m_Position);
}

void Equipment::AddToRenderer(Util::Renderer& renderer) {
    renderer.AddChild(equipmentAnimation);
}

void Equipment::Update(float backgroundSpeed) {
    m_Position.x -= backgroundSpeed; // X 軸移動速度與背景速度一致
    SetPosition(m_Position);        // 更新位置
}

bool Equipment::IsOffScreen() const {
    return m_Position.x < -700.0f;
}

void Equipment::UpdateEquipments(
    float spawnInterval,
    std::vector<std::shared_ptr<Equipment>>& equipments,
    Util::Renderer& renderer,
    float backgroundSpeed)
{
    float deltaTime = Util::Time::GetDeltaTimeMs();
    equipmentSpawnTimer += deltaTime;

    if (equipmentSpawnTimer >= spawnInterval) {
        equipmentSpawnTimer -= spawnInterval;
        auto equipment = std::make_shared<Equipment>();

        // 設置固定的 Y 軸位置，例如 300.0f
        equipment->SetPosition({650.0f, 0.0f}); // 固定 Y 軸位置
        equipment->AddToRenderer(renderer);
        equipments.push_back(equipment);
    }

    for (auto it = equipments.begin(); it != equipments.end(); ) {
        auto& equipment = *it;
        equipment->Update(backgroundSpeed); // 傳遞背景速度

        if (equipment->IsOffScreen()) {
            it = equipments.erase(it);
            renderer.RemoveChild(equipment->equipmentAnimation);
        } else {
            ++it;
        }
    }
}