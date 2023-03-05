#ifndef _LEVEL_OBJECT_H
#define _LEVEL_OBJECT_H

#include "GameObject.h"
#include "Item3D.h"
#include "Level.h"
#include "..\App\app.h"

#include <set>
#include <memory>

const int LEVEL_BUFFER_SIZE = 1000;

const int BLOCK_WIDTH = 212;
const int BLOCK_HEIGHT = 256;

const unsigned int HEX_HEIGHT = 146;
const unsigned int HEX_DEPTH = 84;
const unsigned int HEX_SIDE = 59;
const unsigned int HEX_FACE = 120;
const unsigned int HEX_WIDTH = 238;
// 238 for full length
// 168 for full height (almost exactly 1/sqrt(2) ratio)


class LevelObject : public GameObject
{
public:
    LevelObject(float scale, float xpos, float ypos, int z_index, std::weak_ptr<GameObject> parent);

    void Update(float dt) override;
    void LoadLevel(Level::LEVEL l);

    void SetAngle(float a);


protected:
    Level::LEVEL level;
    float angle;

    void AddBlock(int x, int y, int z, int type);

    std::weak_ptr<Item3D> level_buffer[LEVEL_BUFFER_SIZE];
};

#endif