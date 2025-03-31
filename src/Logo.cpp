#include "Logo.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "config.hpp"

Logo::Logo()
    : m_Image(RESOURCE_DIR "/Image/Home/bgicon.png"),
      m_IsMoving(false) {

    m_Transform.translation = {(WINDOW_WIDTH - m_Image.GetSize().x) / 2 + 35,
                               (WINDOW_HEIGHT - m_Image.GetSize().y) / 2 - 80};

}

void Logo::Update() {
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_IsMoving = true;
    }

    if (m_IsMoving) {
        m_Transform.translation.y += 5.0f;
    }
}

void Logo::Render() {
    Core::Matrices matrices = Util::ConvertToUniformBufferData(m_Transform, m_Image.GetSize(), 0.0f);
    m_Image.Draw(matrices);
}

bool Logo::IsOffScreen() const {
    return m_Transform.translation.y + m_Image.GetSize().y > 1200.f;
}
