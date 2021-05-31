#pragma once
#include "Observer.h"
#include "GameObject.h"


class LevelCompleteObserver :
    public dae::Observer
{
public:
    void OnNotify(const dae::GameObject* actor, dae::Event event) override;

private:
    void Unlock(const dae::GameObject* actor);
};

