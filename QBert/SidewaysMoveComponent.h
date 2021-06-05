#pragma once
#include "GameObject.h"
#include "BaseMoveComponent.h"

class SidewaysMoveComponent final : public BaseMoveComponent
{
public:
    SidewaysMoveComponent(int, bool startSide);
    void Update(float deltaT) override;

private:
    void Descend();
    void ActivateJump(const dae::DirectionSprite& dir) override;
    void Jump(float deltaT) override;
    const dae::DirectionSprite RandomizeDirection() const;
    void FallToDeath(float deltaT) override;
    virtual void CorrectPosition() override;

    bool m_StartSide;
};