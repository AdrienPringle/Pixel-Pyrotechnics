#ifndef _ITEM_3D_H
#define _ITEM_3D_H

#include "SpriteObject.h"
#include "Level.h"

#include <memory>

class Item3D : public SpriteObject
{
public:
    Item3D(float xpos, float ypos, float zpos, float angle, std::weak_ptr<GameObject> parent, std::unique_ptr<CSimpleSprite> sprite);

    void Set3Dposition(float xpos, float ypos, float zpos);
    void SetAngle(float angle);

protected:
    Level::LEVEL level;
    float angle;

    float x_3d;
    float y_3d;
    float z_3d;

    void UpdateScreenCoords();
};

#endif