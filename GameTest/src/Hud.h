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

    void SetState(bool is_down, int level) {
        this->is_down = is_down;
        this->level = level;
    }

protected:

    std::shared_ptr<SpriteObject> level_1_message;
    std::shared_ptr<SpriteObject> level_2_message;
    std::shared_ptr<SpriteObject> level_3_message;

    std::shared_ptr<SpriteObject> congrats_message;

    std::shared_ptr<SpriteObject> restart_message;

    bool is_down;
    int level;

    float screenx;
    float screeny;

};

#endif