#ifndef _HUD_H
#define _HUD_H

#include "GameObject.h"
#include "SpriteObject.h"

#include <memory>

class Hud : public GameObject
{
public:
    Hud(float screenx, float screeny, std::weak_ptr<GameObject> parent);
    void Update(float dt) override;
    void Init();

    void SetLevel(int level)
    {
        if (this->level > level)
        {
            this->level_overflow = true;
        }
        this->level = level;
    }

    void SetIsDown(bool is_down) { this->is_down = is_down; }

    void SetIsActive(bool is_active) { this->is_active = is_active; }

protected:
    void CreateMsg(const char *filename, std::shared_ptr<SpriteObject> &msg);

    void InterpPosition(float goal_x_pos, std::shared_ptr<SpriteObject> msg, float dt);

    std::shared_ptr<SpriteObject> level_1_message;
    std::shared_ptr<SpriteObject> level_2_message;
    std::shared_ptr<SpriteObject> level_3_message;

    std::shared_ptr<SpriteObject> congrats_message;

    std::shared_ptr<SpriteObject> restart_message;

    bool is_down;
    bool is_active;
    bool level_overflow;
    int level;
    int z_count;

    float screenx;
    float screeny;
};

#endif