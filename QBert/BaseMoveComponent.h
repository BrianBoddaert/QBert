#pragma once
#include "ControlComponent.h"
#include "Component.h"
#include "TransformComponent.h"

class BaseMoveComponent : public dae::Component
{
public:
    BaseMoveComponent(int tileIndex)
        : m_Speed{ 50.0f }
        , m_IsMoving{ false }
        , m_StartCubeIndex{ tileIndex }
        , m_CurrentCubeIndex{ tileIndex }
        , m_FallingToDeath{ false }
        , m_Direction{ dae::DirectionSprite::DownLeft }
        , m_JumpCooldownTimer{ 0 }
        , m_JumpCooldown{ 0.5f }
    {};

    virtual void ActivateJump(const dae::DirectionSprite& dir) = 0;
    virtual void Jump(float deltaT) = 0;
    virtual void FallToDeath(float deltaT) = 0;
    bool IsMoving() const { return m_IsMoving;}
    virtual void Update(float deltaT) override = 0;

    void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };
    int GetCurrentCubeIndex()const { return m_CurrentCubeIndex; };
    int GetStartCubeIndex() const { return m_StartCubeIndex; };
    //bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == QBertSprite::UpLeftJump || m_Direction == QBertSprite::UpRightJump); };

protected:
    virtual void CorrectPosition() = 0;

    dae::TransformComponent* m_pTransform;
    bool m_IsMoving;
    bool m_FallingToDeath;
    float m_Speed;
    float m_JumpHeight;
    dae::Vector2 m_SpeedRatiod;
    dae::Vector2 m_MoveDistance;
    bool m_FirstHalfOfTheJump;
    dae::DirectionSprite m_Direction;
    float m_MovementDistanceRatio;
    dae::Vector3 m_JumpStartPos;
    const int m_StartCubeIndex;
    int m_CurrentCubeIndex;

    float m_JumpCooldownTimer;
    float m_JumpCooldown;
};
