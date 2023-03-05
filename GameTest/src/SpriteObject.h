#ifndef _SPRITE_OBJECT_H
#define _SPRITE_OBJECT_H

#include "GameObject.h"
#include "..\App\app.h"

#include <memory>

class SpriteObject : public GameObject
{
public:
    SpriteObject(float scale, float xpos, float ypos, int z_index, std::weak_ptr<GameObject> parent, std::unique_ptr<CSimpleSprite> sprite);

    void Update(float dt) override;
    void Draw() override;

    void SetFrame(unsigned int f);

protected:
    float sprite_scale;

    std::unique_ptr<CSimpleSprite> sprite;
};

#endif