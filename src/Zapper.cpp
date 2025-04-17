#include "Zapper.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Logger.hpp"

Zapper::Zapper(ZapperType type, const glm::vec2& pos)
    : m_Type(type)
{
    // 1) 記住自己的世界座標
    m_Transform.translation = pos;

    // 2) 根據 type 決定要跑哪組圖幀
    std::vector<std::string> paths;
    if (type == ZapperType::VERTICAL) {
        paths = {
            RESOURCE_DIR "/Image/Zapper/ver zapper1.png",
            RESOURCE_DIR "/Image/Zapper/ver zapper2.png",
            RESOURCE_DIR "/Image/Zapper/ver zapper3.png",
            RESOURCE_DIR "/Image/Zapper/ver zapper4.png"
        };
    } else {
        paths = {
            RESOURCE_DIR "/Image/Zapper/zapper1.png",
            RESOURCE_DIR "/Image/Zapper/zapper2.png",
            RESOURCE_DIR "/Image/Zapper/zapper3.png",
            RESOURCE_DIR "/Image/Zapper/zapper4.png"
        };
    }

    // 3) 建立 Util::Animation，設定無限循環、每 100ms 換一幀
    m_Animation = std::make_shared<Util::Animation>(paths, /*play=*/true, /*interval_ms=*/100, /*looping=*/true, /*cooldown=*/0);
    m_Animation->Play();

    // 4) 指定給 GameObject 的 Drawable
    m_Drawable = m_Animation;
}

void Zapper::Update(float /*deltaTime*/) {
    // 不在這裡動作，讓 Camera+Background 去控制整體視差滾動
}

void Zapper::Render() {
    // Draw() 內部會自動呼叫 Update() 跑幀
    Core::Matrices m = Util::ConvertToUniformBufferData(
        m_Transform, 
        m_Animation->GetSize(), 
        0.0f
    );
    m_Animation->Draw(m);
}