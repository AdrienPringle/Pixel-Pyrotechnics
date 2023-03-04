#ifndef _SPRITE_OBJECT_H
#define _SPRITE_OBJECT_H

#include "GameObject.h"
#include "..\App\app.h"

#include <set>
#include <memory>

class SpriteObject: public GameObject
{
public:
    SpriteObject(float scale, float x, float y, std::weak_ptr<GameObject> parent, std::unique_ptr<CSimpleSprite> sprite);
    
    void Update(float dt) override;
    void Draw() override;

protected:
    float sprite_scale;

    std::unique_ptr<CSimpleSprite> sprite;
};

#endif