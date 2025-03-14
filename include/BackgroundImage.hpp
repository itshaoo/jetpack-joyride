//
// Created by hao on 2025/3/12.
//

#ifndef BACKGROUNDIMAGE_H
#define BACKGROUNDIMAGE_H

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "config.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(std::make_unique<Util::Image>(RESOURCE_DIR"/Image/Home/start.png"), -10) {
        resizeToFullScreen();

    }

private:

    void resizeToFullScreen() {
        auto size = Util::Image(RESOURCE_DIR"/Image/Home/start.png").GetSize();
        auto scale = glm::vec2(WINDOW_WIDTH / size.x, WINDOW_HEIGHT / size.y);
        m_Transform.scale = scale;
    }


    
};

#endif //BACKGROUNDIMAGE_H
