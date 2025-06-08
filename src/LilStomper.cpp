#include "LilStomper.hpp"
#include "Animation.hpp"
#include <glm/glm.hpp>

LilStomper::LilStomper() {
    // Running animation
    std::vector<std::string> runPaths = {
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_0_0.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_1_0.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_0.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_1.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_0_1.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_1_1.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_1.png"
    };
    runAnimation = std::make_shared<Animation>(runPaths, /*z=*/5.0f);
    runAnimation->SetLooping(true);
    runAnimation->SetInterval(100);
    runAnimation->Play();

    // Flying/gliding animation
    std::vector<std::string> flyPaths = {
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_2.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_0_3.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_1_3.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_0_4.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_0_5.png"
        
    };
    flyAnimation = std::make_shared<Animation>(flyPaths, /*z=*/5.0f);
    flyAnimation->SetLooping(true);
    flyAnimation->SetInterval(200);
    flyAnimation->Play();

    // Stomp/fall animation
    std::vector<std::string> fallPaths = {
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_3.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_1_4.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_2.png"
    };
    fallAnimation = std::make_shared<Animation>(fallPaths, /*z=*/5.0f);
    fallAnimation->SetLooping(false);
    fallAnimation->SetInterval(100);
    fallAnimation->Play();

    // Flying/gliding animation
    std::vector<std::string> jumpPaths = {
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_2.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_0_3.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_1_4.png",
        RESOURCE_DIR"/Image/Vehicle/lil_stomper/sprite_2_4.png"
    };
    jumpAnimation = std::make_shared<Animation>(jumpPaths, /*z=*/5.0f);
    jumpAnimation->SetLooping(true);
    jumpAnimation->SetInterval(150);
    jumpAnimation->Play();
}

std::shared_ptr<Animation> LilStomper::GetRunAnimation() {
    return runAnimation;
}

std::shared_ptr<Animation> LilStomper::GetFlyAnimation() {
    return flyAnimation;
}

std::shared_ptr<Animation> LilStomper::GetFallAnimation() {
    return fallAnimation;
}

std::shared_ptr<Animation> LilStomper::GetJumpAnimation() {
    return jumpAnimation;
}