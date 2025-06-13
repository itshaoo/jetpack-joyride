#include "missile.hpp"
#include <Util/Time.hpp>
#include "Player.hpp"

float Missile::missileSpawnTimer = 0.0f;

Missile::Missile() {
    std::vector<std::string> missileFrames = {
        RESOURCE_DIR "/Image/Missile/missile0.png",
        RESOURCE_DIR "/Image/Missile/missile1.png",
        RESOURCE_DIR "/Image/Missile/missile2.png",
        RESOURCE_DIR "/Image/Missile/missile3.png",
        RESOURCE_DIR "/Image/Missile/missile4.png",
        RESOURCE_DIR "/Image/Missile/missile5.png"
    };
    missileAnimation = std::make_shared<Animation>(missileFrames, /*z=*/4.0f);
    missileAnimation->SetLooping(true);
    missileAnimation->SetInterval(100);
    missileAnimation->SetVisible(true);
    missileAnimation->Play();

    std::vector<std::string> warningFrames = {
        RESOURCE_DIR "/Image/Warning/warning0.png",
        RESOURCE_DIR "/Image/Warning/warning1.png"
    };
    warningAnimation = std::make_shared<Animation>(warningFrames, /*z=*/4.0f);
    warningAnimation->SetLooping(true);
    warningAnimation->SetInterval(700);
    warningAnimation->SetVisible(true);
    warningAnimation->Play();

    warningSound = std::make_shared<Util::SFX>(RESOURCE_DIR "/Sounds/missile_warning.wav");
    missileSound = std::make_shared<Util::SFX>(RESOURCE_DIR "/Sounds/missile_launch.wav");

}

void Missile::SetTargetPosition(const glm::vec2& targetPosition) {
    m_TargetPosition = targetPosition;
}

void Missile::SetPosition(const glm::vec2& position) {
    m_Position = position;
    missileAnimation->SetPosition(m_Position);
}

void Missile::AddToRenderer(Util::Renderer& renderer) {
    renderer.AddChild(missileAnimation);
    renderer.AddChild(warningAnimation);
}

void Missile::Launch() {
    m_Launched = true;
}

void Missile::SetBarryPositionPtr(const glm::vec2* barryPositionPtr) {
    m_BarryPositionPtr = barryPositionPtr;
}

bool Missile::IsOffScreen() const {
    return m_Position.x < -700.0f; // 假設螢幕左邊界為 -700
}

void Missile::UpdateMissiles(
    float spawnInterval,
    std::vector<std::shared_ptr<Missile>>& missiles,
    Util::Renderer& renderer,
    const glm::vec2& barryPosition,
    float backgroundSpeed)
{
    float deltaTime = Util::Time::GetDeltaTimeMs();
    missileSpawnTimer += deltaTime;

    if (missileSpawnTimer >= spawnInterval) {
        missileSpawnTimer -= spawnInterval;
        auto missile = std::make_shared<Missile>();

        // 設置導彈的目標位置
        missile->SetTargetPosition(barryPosition);
        missile->SetBarryPositionPtr(&barryPosition); // 設置指標

        missile->AddToRenderer(renderer);
        missiles.push_back(missile);
    }

    for (auto it = missiles.begin(); it != missiles.end(); ) {
        auto& missile = *it;
        missile->Update(deltaTime);

        if (missile->IsLaunched()) {
            auto pos = missile->GetPosition();
            pos.x -= backgroundSpeed;
            missile->SetPosition(pos);
        }

        if (missile->IsOffScreen()) {
            it = missiles.erase(it);
            renderer.RemoveChild(missile->missileAnimation);
            renderer.RemoveChild(missile->warningAnimation);
        } else {
            ++it;
        }
    }
}

void Missile::Update(float deltaTime) {
    if (!m_Launched) {
        m_TrackingTimer += deltaTime;

        // 持續更新目標位置
        if (m_BarryPositionPtr) {
            m_TargetPosition.y = m_BarryPositionPtr->y; // 使用 Barry 的 Y 軸作為目標
        }

        glm::vec2 warningPos = { m_Position.x - 50.0f, m_TargetPosition.y };
        if (warningAnimation->GetPosition() != warningPos)
            warningAnimation->SetPosition(warningPos);

        if (!m_WarningSoundPlayed) {
            warningSound->Play(0);
            m_WarningSoundPlayed = true; // 設置標誌，防止重複播放
            }

        // 顯示警告動畫，隱藏導彈動畫
        warningAnimation->SetVisible(true);
        missileAnimation->SetVisible(false);

        if (m_TrackingTimer >= m_TrackingTime) {
            m_Position.y = m_TargetPosition.y;
            SetPosition(m_Position);
            Launch();

            // 隱藏警告動畫，顯示導彈動畫
            warningAnimation->SetVisible(false);
            missileAnimation->SetVisible(true);
        }
    } else {
        m_Position.x -= m_Speed;

        // 播放導彈音效，只播放一次
        if (!m_SoundPlayed) {
            missileSound->Play(0);
            m_SoundPlayed = true; // 設置標誌，防止重複播放
        }

        SetPosition(m_Position);

        // 確保警告動畫隱藏，導彈動畫顯示
        warningAnimation->SetVisible(false);
        missileAnimation->SetVisible(true);
    }
}

void Missile::resetSpawnTimer() {
    missileSpawnTimer = 0.0f;
}