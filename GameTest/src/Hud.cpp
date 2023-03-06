#include "stdafx.h"

#include "Hud.h"
#include <memory>

Hud::Hud(float screenx, float screeny, std::weak_ptr<GameObject> parent) : GameObject(1, 0, 0, 5, parent)
{
    this->screenx = screenx;
    this->screeny = screeny;
    this->level_overflow = false;
}

void Hud::Init()
{
    this->z_count = 0;
    this->CreateMsg(".\\assets\\lvl1_msg.bmp", level_1_message);
    this->CreateMsg(".\\assets\\lvl2_msg.bmp", level_2_message);
    this->CreateMsg(".\\assets\\lvl3_msg.bmp", level_3_message);
    this->CreateMsg(".\\assets\\reset_msg.bmp", restart_message);
    this->CreateMsg(".\\assets\\win_msg.bmp", congrats_message);

    restart_message->SetLocalPosition( -screenx / 2 - 160, screeny / 2 - 200);

    std::unique_ptr<CSimpleSprite> sprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(".\\assets\\bg.bmp", 1, 1));
    sprite->SetFrame(0);
    this->AddChild(std::shared_ptr<SpriteObject>(new SpriteObject(8, 0, 0, -10, shared_from_this(), move(sprite))));
}

void Hud::Update(float dt)
{
    const float RESTING = -screenx / 2 - 160;
    const float ACTIVE = -screenx / 2 + 160;

    this->InterpPosition(level == 0 && is_active? ACTIVE : RESTING, level_1_message, dt);
    this->InterpPosition(level == 1  && is_active? ACTIVE : RESTING, level_2_message, dt);
    this->InterpPosition(level == 2 && this -> is_down && is_active? ACTIVE : RESTING, level_3_message, dt);
    this->InterpPosition(this->is_down && is_active? ACTIVE : RESTING, restart_message, dt);
    this->InterpPosition(this->level_overflow ? ACTIVE : RESTING, congrats_message, dt);

    GameObject::Update(dt);
};

void Hud::CreateMsg(const char *filename, std::shared_ptr<SpriteObject> &msg)
{
    std::unique_ptr<CSimpleSprite> sprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(filename, 1, 1));
    sprite->SetFrame(0);
    msg = std::shared_ptr<SpriteObject>(new SpriteObject(0.25f, screenx / 2 - 160, screeny / 2 - 50, z_count, shared_from_this(), move(sprite)));
    this->AddChild(msg);
    z_count++;
}

void Hud::InterpPosition(float goal_x_pos, std::shared_ptr<SpriteObject> msg, float dt)
{
    float x, y;
    msg->GetLocalPosition(x, y);

    float diff = x - goal_x_pos;

    msg->SetLocalPosition(x - diff * dt / 100, y);
}
