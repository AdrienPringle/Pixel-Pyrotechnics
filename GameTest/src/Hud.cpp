#include "stdafx.h"

#include "Hud.h"
#include <memory>

const float DEG_TO_RAD = 2 * 3.14159f / 360.0f;
const float Z_TO_SCREEN_Y = 1.0f / sqrt(2);

Hud::Hud(float screenx, float screeny, std::weak_ptr<GameObject> parent) : GameObject(1, 0, 0, 5, parent)
{
    this->screenx = screenx;
    this->screeny = screeny;
}

void Hud::Update(float dt){

};
