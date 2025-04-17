#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    static Camera& GetInstance() {
        static Camera instance;
        return instance;
    }
    // 更新攝影機（deltaTime 以秒計算）
    void Update(float deltaTime) {
        m_Offset.x += m_ScrollSpeed * deltaTime;
    }
    const glm::vec2& GetOffset() const { return m_Offset; }
    void SetScrollSpeed(float speed) { m_ScrollSpeed = speed; }
private:
    Camera() : m_Offset(0.0f, 0.0f), m_ScrollSpeed(250.0f) {} // 改為 250 像素/秒
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    glm::vec2 m_Offset;
    float m_ScrollSpeed;
};

#endif // CAMERA_HPP