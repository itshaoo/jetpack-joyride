#ifndef ILEVEL_HPP
#define ILEVEL_HPP

class ILevel {
public:
    virtual ~ILevel() = default;
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;

    virtual bool IsCompleted() const { return false; }
};

#endif