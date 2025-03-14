#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;

    m_BackgroundImage = std::make_shared<BackgroundImage>();
    m_Root.AddChild(m_BackgroundImage);

    m_icon = std::make_shared<Object>(RESOURCE_DIR"/Image/Home/bgicon.png");
    float centerX = ((WINDOW_WIDTH - m_icon->GetScaledSize().x) / 2.0f) +35;
    float centerY = ((WINDOW_HEIGHT - m_icon->GetScaledSize().y) / 2.0f) -80;
    m_icon->SetPosition({centerX, centerY});
    m_icon->SetZIndex(50);
    m_Root.AddChild(m_icon);
}

void App::Update() {
    //TODO: do your things here and delete this line <3
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
