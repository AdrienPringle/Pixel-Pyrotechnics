#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <set>
#include <memory>

class GameObject
{
public:
    GameObject(void);
    GameObject(float scale, float x, float y, std::weak_ptr<GameObject> parent);

    virtual void Update(float dt);
    virtual void Draw();
    virtual void AddChild(std::shared_ptr<GameObject> child);

    void SetLocalPosition(float x, float y);
    void SetScale(float s);

    float GetScale() const { return scale; }
    void GetGlobalPosition(float &x, float &y)
    {
        x = global_xpos;
        y = global_ypos;
    }

protected:
    float scale;
    float xpos;
    float ypos;

    float global_scale;
    float global_xpos;
    float global_ypos;

    std::set<std::shared_ptr<GameObject>> children;
    std::weak_ptr<GameObject> parent;

    void PropagateGlobalPos();
    void PropagateGlobalScale();
};

#endif