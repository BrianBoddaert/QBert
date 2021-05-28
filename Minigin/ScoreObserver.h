#pragma once
#include "Observer.h"
#include "GameObject.h"

namespace dae {
    class ScoreObserver :
        public Observer
    {
    public:
        void OnNotify(const dae::GameObject* actor, Event event) override;

    private:
        void Unlock(const GameObject* actor);
    };

}