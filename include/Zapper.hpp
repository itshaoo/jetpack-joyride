#ifndef ZAPPER_HPP
#define ZAPPER_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include <string>
#include <vector>
#include <glm/glm.hpp>

// 定義 Zapper 的兩種型態
enum class ZapperType {
    VERTICAL,
    HORIZONTAL
};

class Zapper : public Util::GameObject {
public:
    /**
     * @param type       垂直或水平
     * @param pos        初始世界座標
     */
    Zapper(ZapperType type, const glm::vec2& pos);
    ~Zapper() override = default;

    // 空實作（位置由背景滾動控制）
    void Update(float deltaTime);

    // 繪製動畫幀
    void Render();

    // 新增：讓外部可以取得目前位置
    glm::vec2 GetPosition() const {
        return m_Transform.translation;
    }

private:
    ZapperType                             m_Type;
    std::shared_ptr<Util::Animation>      m_Animation;
};

#endif // ZAPPER_HPP