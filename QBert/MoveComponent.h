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
    bool GetIsOnDisc() const { return m_IsOnDisc; }
    std::shared_ptr<dae::GameObject> GetDiscGameObject() { return m_pDiscGameObject; }

    void SetDiscGameObject(std::shared_ptr<dae::GameObject> disc) { m_pDiscGameObject = disc; }
    bool GetJumpingOnDisc() const { return m_JumpingOnDisc; }
private:
    void CorrectPosition() override;
    std::shared_ptr<dae::GameObject> m_pDiscGameObject;
    bool m_JumpingOnDisc;
    bool m_IsOnDisc;

};