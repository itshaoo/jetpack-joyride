#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_BackgroundImage = std::make_shared<BackgroundImage>();
    m_Root.AddChild(m_BackgroundImage);

    m_icon = std::make_shared<Object>(RESOURCE_DIR"/Image/Home/bgicon.png");
    float centerX = ((WINDOW_WIDTH - m_icon->GetScaledSize().x) / 2.0f) +35;
    float centerY = ((WINDOW_HEIGHT - m_icon->GetScaledSize().y) / 2.0f) -80;
    m_icon->SetPosition({centerX, centerY});
    m_icon->SetZIndex(50);
    m_Root.AddChild(m_icon);

    m_moveIcon = false; 
    m_iconOutOfWindow = false;

    // Create the Barry animation
    std::vector<std::string> barryPaths = {
        RESOURCE_DIR"/Image/barry/barry0.png",
        RESOURCE_DIR"/Image/barry/barry1.png"
    };
    m_Barry = std::make_shared<Animation>(barryPaths);
    m_Barry->SetPosition({-220.5f, -265.5f});
    m_Barry->SetLooping(true);
    m_Barry->SetInterval(100);
    m_Barry->Play();
    m_Root.AddChild(m_Barry);

    m_CurrentState = State::UPDATE;

}

void App::Update() {
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        m_moveIcon = true;
    }

    // Move the icon upwards if the flag is set
    if (m_moveIcon) {
        glm::vec2 currentPosition = m_icon->GetPosition();
        currentPosition.y += 10.0f; // Move the icon upwards
        m_icon->SetPosition(currentPosition);

        // Stop moving the icon if it is out of the window
        if (currentPosition.y + m_icon->GetScaledSize().y < 0) {
            m_moveIcon = false;
            m_iconOutOfWindow = true; 
        }
    }

    if (m_iconOutOfWindow) {
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
            // Move Barry upwards
            glm::vec2 barryPosition = m_Barry->GetPosition();
            barryPosition.y += 10.0f;
            m_Barry->SetPosition(barryPosition);
        } else {
            // Return Barry to its original position
            glm::vec2 originalPosition = {-220.5f, -265.5f};
            glm::vec2 barryPosition = m_Barry->GetPosition();
            if (barryPosition.y < originalPosition.y) {
                barryPosition.y -= 7.0f;
                m_Barry->SetPosition(barryPosition);
            }
        }
    }
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
