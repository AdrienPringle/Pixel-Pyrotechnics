#include "stdafx.h"

#include "SpriteObject.h"
#include "..\App\app.h"

#include <math.h>
#include <assert.h>
#include <set>
#include <memory>

LevelObject::LevelObject(float scale, float xpos, float ypos, int z_index, std::weak_ptr<GameObject> parent) : GameObject(scale, xpos, ypos, z_index, parent)
{
    level = Level::LEVEL{0, 0, 0, nullptr};
}

float sigmoid(float x)
{
    return x / (1 + abs(x));
}

void LevelObject::Update(float dt)
{
    float diff_angle = this->angle - (90.0f * this->goal_angle);
    diff_angle = fmod(diff_angle + 180, 360) - 180;

    if (abs(diff_angle) > 0.1f)
    {
        float new_angle = fmod(angle - sigmoid(diff_angle / 45) * dt / 2, 360);
        if (new_angle < 0)
            new_angle += 360;
        this->SetAngle(new_angle);
    }
    else
    {
        this->SetAngle(90.0f * this->goal_angle);
    }

    GameObject::Update(dt);
}

void LevelObject::LoadLevel(Level::LEVEL l)
{
    assert(l.x * l.y * l.z <= LEVEL_BUFFER_SIZE);

    level = l;
    this->children.clear();
    level_state = LevelState::input;
    barrel_count = 0;

    this->AddBomb();

    for (int x = 0; x < level.x; x++)
    {
        for (int y = 0; y < level.y; y++)
        {
            for (int z = 0; z < level.z; z++)
            {
                int i = this->GetBufferIndex(x, y, z);
                int block = level.data[i];

                if (block == Level::BlockType::barrel)
                {
                    barrel_count++;
                }

                if (block > Level::BlockType::air && block < Level::BlockType::man)
                {
                    this->AddBlock(x, y, z, block);
                }
                else if (block == Level::BlockType::man)
                {
                    this->AddMan(x, y, z);
                }
            }
        }
    }
}

void LevelObject::AddMan(int x, int y, int z)
{
    std::unique_ptr<CSimpleSprite> hexSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(Level::textures[Level::BlockType::man], 45, 1));

    float blockx, blocky, blockz;
    this->TileToWorldCoords(x, y, z, blockx, blocky, blockz);
    std::shared_ptr<Item3D> man = std::shared_ptr<Item3D>(new Item3D(blockx, blocky, blockz, 0, shared_from_this(), move(hexSprite)));

    this->AddChild(man);
    this->man_model = man;

    man_x = x;
    man_y = y;
    man_z = z;

    man_x_old = x;
    man_y_old = y;
    man_z_old = z;
}

void LevelObject::AddBomb()
{
    std::unique_ptr<CSimpleSprite> sprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite("..\\assets\\bomb_upscale.bmp", 9, 1));

    // testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
    // testSprite->SetPosition(400.0f, 400.0f);
    // float speed = 1.0f / 15.0f;
    sprite->CreateAnimation(0, 1 / 8.0f, {5, 6, 7, 8, 7, 6, 5, 0});
    // sprite->SetAnimation(0);
    // testSprite->CreateAnimation(ANIM_LEFT, speed, {8, 9, 10, 11, 12, 13, 14, 15});
    // testSprite->CreateAnimation(ANIM_RIGHT, speed, {16, 17, 18, 19, 20, 21, 22, 23});
    // testSprite->CreateAnimation(ANIM_FORWARDS, speed, {24, 25, 26, 27, 28, 29, 30, 31});
    // testSprite->SetScale(1.0f);

    // 152/17

    std::shared_ptr<Item3D> bomb = std::shared_ptr<Item3D>(new Item3D(0.0f, 0.0f, 0.0f, 0, shared_from_this(), move(sprite)));
    bomb->SetFrame(1);
    bomb->SetScale(2);

    this->AddChild(bomb);
    this->bomb_model = bomb;

    this->UpdateBombPos(man_x, man_y, man_z, true);
}

void LevelObject::AddBlock(int x, int y, int z, int type)
{
    int i = this->GetBufferIndex(x, y, z);
    std::unique_ptr<CSimpleSprite> hexSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(Level::textures[type], 45, 1));

    float blockx, blocky, blockz;
    this->TileToWorldCoords(x, y, z, blockx, blocky, blockz);
    std::shared_ptr<Item3D> block_obj = std::shared_ptr<Item3D>(new Item3D(blockx, blocky, blockz, 0, shared_from_this(), move(hexSprite)));

    this->AddChild(block_obj);
    this->level_buffer[i] = block_obj;
}

void LevelObject::TileToWorldCoords(float x, float y, float z, float &wx, float &wy, float &wz)
{
    float by = ((float)y - 0.5f * (float)level.y + 0.5f);
    float bx = ((float)x - 0.5f * (float)level.x + 0.5f);
    float bz = ((float)z - 0.5f * (float)level.z + 0.5f);

    // float bx = ((float)x - 0.5f * (float)level.x + 0.5f);
    // float by = ((float)y - 0.5f * (float)level.y + 0.5f);
    // float bz = ((float)z - 0.5f * (float)level.z + 0.5f);

    // float blockx = bx * (HEX_SIDE + HEX_FACE);
    // float blocky = by * HEX_DEPTH * sqrt(2);
    // float blockz = (bz + 0.5f * bx) * HEX_WIDTH * sqrt(3) / 2;

    wy = by * BLOCK_HEIGHT / 1.8;
    wx = (bx + bz) * BLOCK_WIDTH / 2;
    wz = (bx - bz) * BLOCK_WIDTH / 2;
}

void LevelObject::SetAngle(float a)
{
    this->angle = a;

    int cycles_till_full = 4;
    int max_frames = 45;
    float interp = fmod((angle + 1) / 360.0f, 1);
    int frame = (int)(interp * cycles_till_full * max_frames) % max_frames;

    man_model->SetAngle(a);
    man_model->SetFrame(frame);

    bomb_model->SetAngle(a);

    for (int x = 0; x < level.x; x++)
    {
        for (int y = 0; y < level.y; y++)
        {
            for (int z = 0; z < level.z; z++)
            {
                std::shared_ptr<Item3D> block;
                if (this->GetBlockAt(x, y, z, block))
                {
                    block->SetAngle(a);
                    block->SetFrame(frame);
                }
            }
        }
    }
}
bool LevelObject::InRangeAt(int x, int y, int z)
{
    return !(x < 0 || y < 0 || z < 0 || x >= level.x || y >= level.y || z >= level.z);
}

void LevelObject::DeleteBlockAt(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 || x >= level.x || y >= level.y || z >= level.z)
        return;

    int i = GetBufferIndex(x, y, z);
    std::weak_ptr<Item3D> block = this->level_buffer[i];
    if (auto tmp = block.lock())
    {
        this->level_buffer[i] = std::shared_ptr<Item3D>(nullptr);
        children.erase(std::remove(children.begin(), children.end(), tmp), children.end());
    }
}

bool LevelObject::GetBlockAt(int x, int y, int z, std::shared_ptr<Item3D> &block)
{
    if (x < 0 || y < 0 || z < 0 || x >= level.x || y >= level.y || z >= level.z)
        return false;

    int i = GetBufferIndex(x, y, z);
    if(auto tmp = this->level_buffer[i].lock()){
        block = tmp;
        return true;
    }
    return false;
}
Level::BlockType LevelObject::GetBlockTypeAt(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 || x >= level.x || y >= level.y || z >= level.z)
        return Level::BlockType::air;

    int i = GetBufferIndex(x, y, z);
    return static_cast<Level::BlockType>(level.data[i]);
}

int LevelObject::GetBufferIndex(int x, int y, int z)
{
    return y + x * level.y + z * level.y * level.x;
}

void LevelObject::RotateRight()
{
    this->goal_angle = (goal_angle + 1) % 4;
}

void LevelObject::RotateLeft()
{
    this->goal_angle = (goal_angle - 1) % 4;
}

void LevelObject::GetManScreenPos(float &x, float &y)
{
    this->man_model->GetGlobalPosition(x, y);
    y -= 0.25 * BLOCK_HEIGHT / 1.8;
}

void LevelObject::GetManPos(int &x, int &y, int &z)
{
    x = man_x;
    y = man_y;
    z = man_z;
}

void LevelObject::UpdateManPos(int x, int y, int z)
{
    man_x_old = man_x;
    man_y_old = man_y;
    man_z_old = man_z;

    man_x = x;
    man_y = y;
    man_z = z;

    float blockx, blocky, blockz;
    this->TileToWorldCoords(x, y, z, blockx, blocky, blockz);

    this->man_model->Set3Dposition(blockx, blocky, blockz);
}

void LevelObject::UpdateBombPos(int x, int y, int z, bool is_valid)
{
    if (!is_valid)
    {
        this->bomb_model->SetFrame(0);
        return;
    }
    this->bomb_model->SetFrame(1);
    bomb_x = x;
    bomb_y = y;
    bomb_z = z;

    float blockx, blocky, blockz;
    this->TileToWorldCoords(x, y, z, blockx, blocky, blockz);

    this->bomb_model->Set3Dposition(blockx, blocky, blockz);
}