#pragma once
#include "Component.h"
#include <SDL.h>
#include "PlayerComponent.h"

namespace dae 
{
    
    class MoveComponent final : public Component
    {
    public:
        MoveComponent();
        void ActivateJump(const QBertSprite& dir);
        void Jump(float deltaT);
        void FallToDeath(float deltaT);
        bool IsMoving() const;
        void Update(float deltaT) override;

        void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };

        bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump &&(m_Direction == QBertSprite::UpLeftJump || m_Direction == QBertSprite::UpRightJump); };
    private:
        bool m_IsMoving;
        bool m_FallingToDeath;
        float m_Speed;
        glm::vec2 m_MoveDistance;
        bool m_FirstHalfOfTheJump;
        QBertSprite m_Direction;

        glm::vec2 m_JumpStartPos;
        int m_CurrentCubeIndex;
    };

}