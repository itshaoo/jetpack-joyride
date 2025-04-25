#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <Util/Renderer.hpp>
#include <Util/Time.hpp>
#include <Animation.hpp>
#include <Util/BGM.hpp>

class Missile {
public:
    Missile();

    void SetTargetPosition(const glm::vec2& targetPosition);
    void SetPosition(const glm::vec2& position);
    void SetBarryPositionPtr(const glm::vec2* barryPositionPtr);  // New method to set Barry's position pointer
    void AddToRenderer(Util::Renderer& renderer);
    void Launch();

    void Update(float deltaTime);
    bool IsOffScreen() const;

    static void UpdateMissiles(float spawnInterval,
                               std::vector<std::shared_ptr<Missile>>& missiles,
                               Util::Renderer& renderer,
                               const glm::vec2& barryPosition);

    glm::vec2 GetPosition() const { return m_Position; }
    glm::vec2 GetSize()     const { return missileAnimation->GetScaledSize(); }

private:
    glm::vec2 m_Position = {650.0f, 0.0f};
    glm::vec2 m_TargetPosition{};
    const glm::vec2* m_BarryPositionPtr = nullptr; // Pointer to Barry's position
    float m_TrackingTime = 1000.0f; 
    float m_TrackingTimer = 0.0f;

    float m_Speed = 15.0f;
    bool m_Launched = false;
    std::shared_ptr<Util::BGM> missileSound;
    std::shared_ptr<Util::BGM> warningSound;

    bool m_WarningSoundPlayed = false;
    bool m_SoundPlayed = false;

    std::shared_ptr<Animation> missileAnimation;
    std::shared_ptr<Animation> warningAnimation;

    static float missileSpawnTimer;
};