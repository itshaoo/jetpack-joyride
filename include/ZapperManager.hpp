#ifndef ZAPPER_MANAGER_HPP
#define ZAPPER_MANAGER_HPP

#include "Zapper.hpp"
#include "Util/Renderer.hpp"
#include <vector>
#include <memory>

class ZapperManager {
public:
    ZapperManager();
    ~ZapperManager() = default;

    /**
     * @brief 每一幀呼叫，用來處理障礙物的產生與清理
     * @param deltaTime 兩針更新間隔（秒）
     */
    void Update(float deltaTime);

    /**
     * @brief 如果有額外非 Renderer 管理的繪製需求，可在這裡實作
     */
    void Render();

    /**
     * @brief 指定 Renderer，讓每隻 Zapper 自動加入渲染管線
     */
    void SetRenderer(Util::Renderer* renderer) { m_Renderer = renderer; }

private:
    /**
     * @brief 生成一批新的 Zapper
     */
    void SpawnZappers();

private:
    std::vector<std::shared_ptr<Zapper>> m_Zappers;  // 目前所有障礙物
    float m_SpawnTimer;      // 計時用
    float m_SpawnInterval;   // 下次生成間隔（秒）
    float m_MinY;            // 障礙物可出現的最低高度
    float m_MaxY;            // 障礙物可出現的最高高度

    Util::Renderer* m_Renderer = nullptr;  // 外部注入的 Renderer
};

#endif // ZAPPER_MANAGER_HPP