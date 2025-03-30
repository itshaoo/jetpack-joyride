#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Player.hpp"

void App::Start() {
    LOG_TRACE("Start");

    Render();

    m_Player = std::make_shared<Player>();
    m_Player->AddToRenderer(m_Root);

    m_CurrentState = State::UPDATE;
}

void App::Render() {
    if (!m_BackgroundStarted) {
        m_Logo.Render();
    }
    m_Background.Render();
}

void App::Update() {
    m_Logo.Update();

    m_isSpacePressed = Util::Input::IsKeyPressed(Util::Keycode::SPACE);

    if (!m_BackgroundStarted && m_Logo.IsOffScreen()) {
        m_BackgroundStarted = true;
    }

    if (m_BackgroundStarted) {
        m_Background.Update();

        m_Player->Update();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}