#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <vector>
#include <memory>

class GameObject
{
public:
    GameObject(void);
    GameObject(float scale, float x, float y, int z_index, std::weak_ptr<GameObject> parent);

    virtual void Update(float dt);
    virtual void Draw();
    virtual void AddChild(std::shared_ptr<GameObject> child);
    
    void SetLocalPosition(float x, float y);
    void SetScale(float s);
    void SetZindex(int i);

protected:
    // order to draw children is decided first by z index
    struct cmpStruct
    {
        bool operator()(std::shared_ptr<GameObject> lhs, std::shared_ptr<GameObject> rhs) const
        {
            int z_l = lhs->GetZindex();
            int z_r = rhs->GetZindex();
            if(z_l == z_r) return lhs.get() < rhs.get();
            return z_l < z_r;
        }
    } CMP_STRUCT;

    float scale;
    float xpos;
    float ypos;
    int z_index;

    float global_scale;
    float global_xpos;
    float global_ypos;

    std::vector<std::shared_ptr<GameObject>> children;
    std::weak_ptr<GameObject> parent;

    int GetZindex() { return z_index; }
    float GetScale() const { return scale; }
    void GetGlobalPosition(float &x, float &y)
    {
        x = global_xpos;
        y = global_ypos;
    }

    void PropagateGlobalPos();
    void PropagateGlobalScale();
};

#endif