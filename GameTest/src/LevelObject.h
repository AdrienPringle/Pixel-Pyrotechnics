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

enum LevelState
{
    input,
    transition,
    load
};

enum ResetType{
    win, 
    manual,
};

class LevelObject : public GameObject
{
public:
    LevelObject(float scale, float xpos, float ypos, int z_index, std::weak_ptr<GameObject> parent);

    void Update(float dt) override;
    void LoadLevel(Level::LEVEL l);
    void UpdateLevel(Level::LEVEL l, ResetType type);

    void RotateLeft();
    void RotateRight();

    void UpdateManPos(int x, int y, int z, bool move);
    void UpdateBombPos(int x, int y, int z, bool is_valid);
    void ReduceBarrelCount() { barrel_count--; }

    bool InRangeAt(int x, int y, int z);
    void DeleteBlockAt(int x, int y, int z);
    bool GetBlockAt(int x, int y, int z, std::shared_ptr<Item3D> &block);
    Level::BlockType GetBlockTypeAt(int x, int y, int z);

    void GetManPos(int &x, int &y, int &z);
    void GetManScreenPos(float &x, float &y);
    int GetGoalAngle() { return goal_angle; };
    int GetBarrelCount() { return barrel_count; };
    LevelState GetLevelState() { return level_state; };

protected:
    Level::LEVEL level;
    Level::LEVEL buffer_level;
    bool level_loaded;
    ResetType reset_type;

    LevelState level_state;
    float angle;
    int goal_angle; // 0 to 3 (every 90 degrees)

    void HandleTransition(float dt);
    void HandleLevelTransitionManual(float dt);
    void HandleLevelTransitionWin(float dt);

    void SetAngle(float a);
    void AddBlock(int x, int y, int z, int type);
    void AddMan(int x, int y, int z);
    void AddBomb();
    void TileToWorldCoords(float x, float y, float z, float &wx, float &wy, float &wz);

    int GetBufferIndex(int x, int y, int z);

    int barrel_count;

    bool bomb_valid;
    bool man_move;
    int bomb_x, bomb_y, bomb_z;
    int man_x, man_y, man_z;
    int man_x_old, man_y_old, man_z_old;
    float transition_interp;

    std::weak_ptr<Item3D> level_buffer[LEVEL_BUFFER_SIZE];

    std::shared_ptr<Item3D> man_model;
    std::shared_ptr<Item3D> bomb_model;
};

#endif