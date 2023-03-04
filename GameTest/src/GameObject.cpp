#include "stdafx.h"
#include "GameObject.h"

#include <set>
#include <memory>

GameObject::GameObject(void)
{
    this->SetScale(1.0f);
    this->SetLocalPosition(0, 0);
    parent = std::weak_ptr<GameObject>();
}

GameObject::GameObject(float scale, float xpos, float ypos, std::weak_ptr<GameObject> parent) : parent(parent)
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
    for (auto c : children)
    {
        c->Draw();
    }
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    children.insert(child);
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
        float scale = p->GetScale();
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
        global_scale = scale * p->GetScale();
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
