#pragma once
#include "BaseMoveComponent.h"
#include <SDL.h>
#include "ControlComponent.h"
#include "TransformComponent.h"

class MoveComponent final : public BaseMoveComponent
{
public:
    MoveComponent(int tileIndex);
    void ActivateJump(const dae::DirectionSprite& dir) override;
    void Jump(float deltaT) override;
    void FallToDeath(float deltaT) override;
    void Update(float deltaT) override;

    void JumpOnDisc(float deltaT);

    dae::TransformComponent* GetDiscTransform() { return m_pDiscTransform; }

    void SetDiscTransform(dae::TransformComponent* discTC) { m_pDiscTransform = discTC; }
    bool GetJumpingOnDisc() const { return m_JumpingOnDisc; }
private:
    void CorrectPosition() override;
    dae::TransformComponent* m_pDiscTransform;
    bool m_JumpingOnDisc;
    bool m_IsOnDisc;

};