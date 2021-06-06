#pragma once
#include <Component.h>
#include "ControlComponent.h"

class PathFindingComponent :
    public dae::Component
{
public:
    PathFindingComponent();
    void Update(float) override;
    dae::DirectionSprite FindDirectionToClosestPlayer() const;
private:
    const std::shared_ptr<dae::GameObject> FindClosestPlayer() const;
    dae::DirectionSprite DescendRandomDirection() const;
    float GetDistanceBetween(const dae::Vector3& a, const dae::Vector3& b) const;

};

