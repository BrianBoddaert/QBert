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
        MoveComponent(int);
        void ActivateJump(const QBertSprite& dir);
        void Jump(float deltaT);
        void FallToDeath(float deltaT);
        bool IsMoving() const;
        void Update(float deltaT) override;

        void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };
        int GetStartCubeIndex() const { return m_StartCubeIndex; };
        bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == QBertSprite::UpLeftJump || m_Direction == QBertSprite::UpRightJump); };
    private:
        void CorrectPosition();

        dae::TransformComponent* m_pTransform;
        bool m_IsMoving;
        bool m_FallingToDeath;
        float m_Speed;
        float m_JumpHeight;
        glm::vec2 m_SpeedRatiod;
        glm::vec2 m_MoveDistance;
        bool m_FirstHalfOfTheJump;
        QBertSprite m_Direction;
        float m_MovementDistanceRatio;
        glm::vec2 m_JumpStartPos;
        const int m_StartCubeIndex;
        int m_CurrentCubeIndex;
    };

}