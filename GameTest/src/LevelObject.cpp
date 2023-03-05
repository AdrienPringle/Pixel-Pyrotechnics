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

void LevelObject::Update(float dt)
{
    GameObject::Update(dt);
}

void LevelObject::LoadLevel(Level::LEVEL l)
{
    assert(l.x * l.y * l.z <= LEVEL_BUFFER_SIZE);

    level = l;
    this->children.clear();

    for (int x = 0; x < level.x; x++)
    {
        for (int y = 0; y < level.y; y++)
        {
            for (int z = 0; z < level.z; z++)
            {
                int i = y + x * level.y + z * level.y * level.x;
                int block = level.data[i];

                if (block > 0 )
                {
                    this->AddBlock(x, y, z, block);
                }
            }
        }
    }
}

void LevelObject::AddBlock(int x, int y, int z, int type)
{
    int i = y + x * level.y + z * level.y * level.x;

    float by = ((float)y - 0.5f * (float)level.y + 0.5f);

    float bx = ((float)x - 0.5f * (float)level.x + 0.5f);
    float bz = ((float)z - 0.5f * (float)level.z + 0.5f);

    float blocky = by * BLOCK_HEIGHT / 1.8;
    float blockx = (bx + bz) * BLOCK_WIDTH / 2;
    float blockz = (bx - bz) * BLOCK_WIDTH / 2;

    // float bx = ((float)x - 0.5f * (float)level.x + 0.5f);
    // float by = ((float)y - 0.5f * (float)level.y + 0.5f);
    // float bz = ((float)z - 0.5f * (float)level.z + 0.5f);

    // float blockx = bx * (HEX_SIDE + HEX_FACE);
    // float blocky = by * HEX_DEPTH * sqrt(2);
    // float blockz = (bz + 0.5f * bx) * HEX_WIDTH * sqrt(3) / 2;

    const char *textures[] = {
        "..\\assets\\blocks.bmp",
        "..\\assets\\bricks.bmp",
        "..\\assets\\door.bmp",
        "..\\assets\\barrel.bmp",
    };

    std::unique_ptr<CSimpleSprite> hexSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(
        textures[type - 1], 45, 1));

    std::shared_ptr<Item3D> block_obj = std::shared_ptr<Item3D>(new Item3D(blockx, blocky, blockz, 0, shared_from_this(), move(hexSprite)));

    this->AddChild(block_obj);
    this->level_buffer[i] = block_obj;
}

void LevelObject::SetAngle(float a)
{
    this->angle = a;

    int cycles_till_full = 4;
    int max_frames = 45;
    float interp = angle / 360.0f;
    int frame = (int)(interp * cycles_till_full * max_frames) % max_frames;

    for (int x = 0; x < level.x; x++)
    {
        for (int y = 0; y < level.y; y++)
        {
            for (int z = 0; z < level.z; z++)
            {
                int i = y + x * level.y + z * level.y * level.x;
                std::weak_ptr<Item3D> block = this->level_buffer[i];

                if (auto tmp = block.lock())
                {
                    tmp->SetAngle(a);
                    tmp->SetFrame(frame);
                }
            }
        }
    }
}