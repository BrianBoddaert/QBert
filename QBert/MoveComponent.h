#pragma once
#include "Component.h"
#include <SDL.h>
#include "ControlComponent.h"
#include "TransformComponent.h"

namespace dae
{

    class MoveComponent final : public Component
    {
    public:
        MoveComponent(int tileIndex);
        void ActivateJump(const QBertSprite& dir);
        void Jump(float deltaT);
        void FallToDeath(float deltaT);
        bool IsMoving() const;
        void Update(float deltaT) override;

        void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };
        int GetCurrentCubeIndex()const { return m_CurrentCubeIndex; };
        int GetStartCubeIndex() const { return m_StartCubeIndex; };
        bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == QBertSprite::UpLeftJump || m_Direction == QBertSprite::UpRightJump); };
    private:
        void CorrectPosition();

        dae::TransformComponent* m_pTransform;
        bool m_IsMoving;
        bool m_FallingToDeath;
        float m_Speed;
        float m_JumpHeight;
        dae::Vector2 m_SpeedRatiod;
        dae::Vector2 m_MoveDistance;
        bool m_FirstHalfOfTheJump;
        QBertSprite m_Direction;
        float m_MovementDistanceRatio;
        dae::Vector3 m_JumpStartPos;
        const int m_StartCubeIndex;
        int m_CurrentCubeIndex;

        float m_JumpCooldownTimer;
        float m_JumpCooldown;
    };

}