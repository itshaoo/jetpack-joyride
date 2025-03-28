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
    m_isSpacePressed = false;

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

    m_BarryFly = std::make_shared<Animation>(std::vector<std::string>{
        RESOURCE_DIR"/Image/barry/barryflying0.png",
        RESOURCE_DIR"/Image/barry/barryflying1.png",
        RESOURCE_DIR"/Image/barry/barryflying2.png",
    });
    m_BarryFly->SetPosition({-220.5f, -265.5f});
    m_BarryFly->SetLooping(true);
    m_BarryFly->SetInterval(100);
    m_BarryFly->Play();
    m_BarryFly->SetVisible(false); // Start hidden
    m_Root.AddChild(m_BarryFly);

    m_BarryFall = std::make_shared<Animation>(std::vector<std::string>{
        RESOURCE_DIR"/Image/barry/barryfalling.png"
    });
    m_BarryFall->SetPosition({-220.5f, -265.5f});
    m_BarryFall->SetLooping(true);
    m_BarryFall->SetInterval(100);
    m_BarryFall->Play();
    m_BarryFall->SetVisible(false); // Start hidden
    m_Root.AddChild(m_BarryFall);

    m_CurrentState = State::UPDATE;

}

void App::Update() {
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        m_moveIcon = true;
    }

    m_isSpacePressed = Util::Input::IsKeyPressed(Util::Keycode::SPACE);

    if (m_moveIcon) {
        glm::vec2 currentPosition = m_icon->GetPosition();
        currentPosition.y += 10.0f;
        m_icon->SetPosition(currentPosition);

        if (currentPosition.y + m_icon->GetScaledSize().y > 1100) {
            m_moveIcon = false;
            m_iconOutOfWindow = true;
        }
    }

    if (m_iconOutOfWindow) {
        glm::vec2 barryPosition = m_Barry->GetPosition();
        glm::vec2 originalPosition = {-220.5f, -265.5f}; // Barry 的原始位置（地面）

        if (m_isSpacePressed) {
            // Barry 上升，但不能超過最大高度
            if (barryPosition.y < 250.0f) {
                barryPosition.y += 9.5f; // 移動上升
            }

            // 切換到飛行動畫
            m_Barry->SetVisible(false);
            m_BarryFly->SetVisible(true);
            m_BarryFall->SetVisible(false);
        } 
        else {
            // Barry 下降
            if (barryPosition.y > originalPosition.y) {
                barryPosition.y -= 9.5f;

                // Barry 在下降過程中，應該顯示「下降動畫」
                m_Barry->SetVisible(false);
                m_BarryFly->SetVisible(false);
                m_BarryFall->SetVisible(true);
            }

            // Barry 落地（回到原始位置）
            if (barryPosition.y <= originalPosition.y) {
                barryPosition.y = originalPosition.y; // 確保 Barry 停在地面
                m_Barry->SetVisible(true);
                m_BarryFly->SetVisible(false);
                m_BarryFall->SetVisible(false);
            }
        }

        // 更新 Barry 的位置
        m_Barry->SetPosition(barryPosition);
        m_BarryFly->SetPosition(barryPosition);
        m_BarryFall->SetPosition(barryPosition);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}


void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
