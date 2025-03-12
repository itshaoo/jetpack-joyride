#include "App.hpp"

#include "Core/Context.hpp"
#include "Util/Input.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    context->SetWindowIcon("../Resources/logo/icon.jpg");

    while (!context->GetExit())
    {
        switch (app.GetCurrentState()) {
        case App::State::START:
            app.Start();
            break;

        case App::State::UPDATE:
            app.Update();
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
