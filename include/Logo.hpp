#ifndef LOGO_HPP
#define LOGO_HPP

#include "pch.hpp"
#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"

class Logo {
public:
    Logo();

    void Update();
    void Render();

    bool IsOffScreen() const;

private:
    Util::Image m_Image;
    Util::Transform m_Transform;
    bool m_IsMoving;
};

#endif
