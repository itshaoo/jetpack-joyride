#ifndef COINCOUNTER_HPP
#define COINCOUNTER_HPP

#include "Util/GameObject.hpp"
#include "Object.hpp"

class CoinCounter : public Util::GameObject {
public:
    CoinCounter();

    void SetCount(int count);
};

#endif