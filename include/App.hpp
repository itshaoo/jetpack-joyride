#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Logo.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "Object.hpp"
#include "Animation.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void Render();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    Logo m_Logo;
    Background m_Background;
    bool m_BackgroundStarted = false;

    Util::Renderer m_Root;

    bool m_isSpacePressed = false;

    std::shared_ptr<Player> m_Player;
};

#endif