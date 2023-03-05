#include "stdafx.h"

#include "SpriteObject.h"

#include <set>
#include <memory>

SpriteObject::SpriteObject(float scale, float xpos, float ypos, int z_index, std::weak_ptr<GameObject> parent, std::unique_ptr<CSimpleSprite> sprite) : GameObject(scale, xpos, ypos, z_index, parent)
{
    sprite_scale = sprite->GetScale();
    this->sprite = move(sprite);
}

void SpriteObject::Update(float dt)
{
    GameObject::Update(dt);
    sprite->SetPosition(global_xpos, global_ypos);
    sprite->SetScale(global_scale * sprite_scale);
    sprite->Update(dt);
}

void SpriteObject::Draw()
{   
    GameObject::Draw();
    sprite->Draw();
}

void SpriteObject::SetFrame(unsigned int f)
{
    sprite->SetFrame(f);
}
