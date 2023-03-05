#include "stdafx.h"
#include "GameObject.h"

#include <vector>
#include <memory>
#include <algorithm> 

GameObject::GameObject(void)
{
    this->SetScale(1.0f);
    this->SetLocalPosition(0, 0);
    z_index = 0;
    parent = std::weak_ptr<GameObject>();
}

GameObject::GameObject(float scale, float xpos, float ypos, int z_index, std::weak_ptr<GameObject> parent) : parent(parent), z_index(z_index)
{
    this->SetScale(scale);
    this->SetLocalPosition(xpos, ypos);
}

void GameObject::Update(float dt)
{
    for (auto c : children)
    {
        c->Update(dt);
    }
}

void GameObject::Draw()
{
    std::sort(children.begin(), children.end(), CMP_STRUCT);
    for (auto c : children)
    {
        c->Draw();
    }
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    children.push_back(child);
}

void GameObject::SetZindex(int i){
    z_index = i;
}

void GameObject::SetLocalPosition(float x, float y)
{
    xpos = x;
    ypos = y;

    this->PropagateGlobalPos();
}

void GameObject::SetScale(float s)
{
    scale = s;

    this->PropagateGlobalScale();
}


void GameObject::PropagateGlobalPos()
{
    if (std::shared_ptr<GameObject> p = parent.lock())
    {
        float scale = p->global_scale;
        p->GetGlobalPosition(global_xpos, global_ypos);

        global_xpos += scale * xpos;
        global_ypos += scale * ypos;
    }
    else
    {
        // parent doesn't exist, must be root node
        global_xpos = xpos;
        global_ypos = ypos;
    }

    for (auto c : children)
    {
        c->PropagateGlobalPos();
    }
}

void GameObject::PropagateGlobalScale()
{
    if (std::shared_ptr<GameObject> p = parent.lock())
    {
        global_scale = scale * p->global_scale;
    }
    else
    {
        // parent doesn't exist, must be root node
        global_scale = scale;
    }

    for (auto c : children)
    {
        c->PropagateGlobalScale();
    }
}
