#include "stdafx.h"

#include "SpriteObject.h"

#include <set>
#include <memory>

SpriteObject::SpriteObject(float scale, float xpos, float ypos, std::weak_ptr<GameObject> parent, std::unique_ptr<CSimpleSprite> sprite) : GameObject(scale, xpos, ypos, parent)
{
    sprite_scale = sprite->GetScale();
    this->sprite = move(sprite);
}

void SpriteObject::Update(float dt)
{
    GameObject::Update(dt);
    sprite->SetPosition(global_xpos, global_ypos);
    sprite->SetScale(scale * sprite_scale);
}

void SpriteObject::Draw()
{   
    GameObject::Draw();
    sprite->Draw();
}
