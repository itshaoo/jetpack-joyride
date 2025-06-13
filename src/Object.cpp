#include "Object.hpp"
#include "Util/Image.hpp"


Object::Object(const std::string& ImagePath) {
    SetImage(ImagePath);

    ResetPosition();
}

void Object::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;

    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}