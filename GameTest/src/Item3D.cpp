#include "stdafx.h"

#include "Item3D.h"

#include <math.h>
#include <set>
#include <memory>

const float DEG_TO_RAD = 2 * 3.14159f / 360.0f;
const float Z_TO_SCREEN_Y = 1.0f / sqrt(2);

// x is left right
// z is in out
// y is up down
Item3D::Item3D(float x, float y, float z, float angle, std::weak_ptr<GameObject> parent, std::unique_ptr<CSimpleSprite> sprite) : SpriteObject(1.0f, 0.0f, 0.0f, 0, parent, move(sprite))
{
    this->Set3Dposition(x, y, z);
    this->SetAngle(angle);
}

void Item3D::Set3Dposition(float x, float y, float z)
{
    x_3d = x;
    y_3d = y;
    z_3d = z;

    this->UpdateScreenCoords();
}
void Item3D::SetAngle(float angle){
    this->angle = angle;

    this->UpdateScreenCoords();
}

void Item3D::UpdateScreenCoords() {
    float newy = y_3d;

    float sin_a = sin(angle * DEG_TO_RAD);
    float cos_a = cos(angle * DEG_TO_RAD);

    float newx = x_3d * cos_a - z_3d * sin_a;
    float newz = x_3d * sin_a + z_3d * cos_a;

    this->SetLocalPosition(newx, (newy + newz) * Z_TO_SCREEN_Y);
    this->SetZindex(-newz + newy);
    
}