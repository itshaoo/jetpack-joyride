#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();

    // 1) 初始化 SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // 2) 分配更多音效 channel，確保可以同時播放多條 SFX
    Mix_AllocateChannels(32);
    // （可選）保留最前面的 channel 不給 Mix_PlayChannel 自動使用
    Mix_ReserveChannels(1);

    App app;

    context->SetWindowIcon(RESOURCE_DIR"/Image/logo/icon.png");

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::LEVEL_SELECT:
            case App::State::MISSION_DESCRIPTION:
            case App::State::LEVEL1:
            case App::State::LEVEL2:
            case App::State::LEVEL3:
            case App::State::LEVEL4:
            case App::State::LEVEL5:
            case App::State::LEVEL6:
            case App::State::LEVEL7:
            case App::State::LEVEL8:
            case App::State::LEVEL9:
            case App::State::LEVEL10:
            case App::State::UPDATE:
            case App::State::PAUSED:
            case App::State::GAMEOVER:
                app.Update();
            app.Render();
            break;
            case App::State::START:
                app.Start();
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