#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "BackgroundImage.hpp"
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

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    
    Util::Renderer m_Root;
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

    std::shared_ptr<Object> m_icon;
    bool m_moveIcon;
    bool m_iconOutOfWindow;
    bool m_isSpacePressed;

    std::shared_ptr<Animation> m_Barry;
    std::shared_ptr<Animation> m_BarryFly;
    std::shared_ptr<Animation> m_BarryFall;




};

#endif
