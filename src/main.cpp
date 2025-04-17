#include "App.hpp"
#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    context->SetWindowIcon(RESOURCE_DIR"/Image/logo/icon.png");

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
            break;

            case App::State::UPDATE:
                app.Update();
            app.Render();
            break;

            case App::State::END:
                app.End();
            context->SetExit(true);
            break;
        }
        context->Update();
    }
    return 0;
}