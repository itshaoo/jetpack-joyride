#ifndef COINGROUP_HPP
#define COINGROUP_HPP

#include "Util/GameObject.hpp"
#include <vector>
#include <memory>

class CoinGroup : public Util::GameObject {
public:
    /**
     * @brief Construct a new Coin Group object.
     * @param patternType 0 表示矩形排列，1 表示階梯排列。
     */
    CoinGroup(int patternType);
    virtual ~CoinGroup() = default;

    // 提供存取子物件的方法（供後續 CoinManager 做座標調整）
    const std::vector<std::shared_ptr<Util::GameObject>>& GetChildren() const { return m_Children; }
};

#endif // COINGROUP_HPP