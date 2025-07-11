#include "GravitySuit.hpp"
#include "Animation.hpp"
#include <glm/glm.hpp>

GravitySuit::GravitySuit() {
    std::vector<std::string> runPaths = {
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_2_4.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_3_4.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_0_0.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_2_0.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_2_1.png"
    };
    runAnimation = std::make_shared<Animation>(runPaths, /*z=*/5.0f);
    runAnimation->SetLooping(true);
    runAnimation->SetInterval(100);
    runAnimation->Play();

    std::vector<std::string> flyPaths = {
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_1_2.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_2_2.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_3_2.png"
    };
    flyAnimation = std::make_shared<Animation>(flyPaths, /*z=*/5.0f);
    flyAnimation->SetLooping(false);
    flyAnimation->SetInterval(120);
    flyAnimation->Play();

    std::vector<std::string> fallPaths = {
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_2_3.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_1_3.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_2_1.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_3_1.png"
    };
    fallAnimation = std::make_shared<Animation>(fallPaths, /*z=*/5.0f);
    fallAnimation->SetLooping(false);
    fallAnimation->SetInterval(100);
    fallAnimation->Play();

    std::vector<std::string> topPaths = {
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_3_2.png",
        RESOURCE_DIR"/Image/Vehicle/gravity_suit/sprite_3_3.png"
    };
    topAnimation = std::make_shared<Animation>(topPaths, /*z=*/5.0f);
    topAnimation->SetLooping(true);
    topAnimation->SetInterval(100);
    topAnimation->Play();
}

std::shared_ptr<Animation> GravitySuit::GetRunAnimation() {
    return runAnimation;
}

std::shared_ptr<Animation> GravitySuit::GetFlyAnimation() {
    return flyAnimation;
}

std::shared_ptr<Animation> GravitySuit::GetFallAnimation() {
    return fallAnimation; 
}

std::shared_ptr<Animation> GravitySuit::GetTopAnimation() {
    return topAnimation;
}