#include "MiniginPCH.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include <math.h>
#include "SceneManager.h"
#include "Scene.h"
#include "MapComponent.h"
#include "HealthComponent.h"

using namespace dae;


MoveComponent::MoveComponent()
    : m_Speed{ 50 }
    , m_IsMoving{ false }
    , m_CurrentCubeIndex{ 0 }
    , m_FallingToDeath{ false }
    , m_Direction{ QBertSprite::DownLeft }
{
    const glm::vec2& cubeOffset = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetCubeOffset();
    m_MoveDistance = cubeOffset * GAMESCALE;
}

void MoveComponent::ActivateJump(const QBertSprite& dir)
{
    const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();
    m_pTransform = m_pGameObject->GetComponent<TransformComponent>();
    renderComp->SetSrcRect(SDL_Rect{ (int)dir * (int)renderComp->GetSpritePixelSize().x,0,(int)renderComp->GetSpritePixelSize().x,(int)renderComp->GetSpritePixelSize().y });

    m_JumpStartPos = m_pTransform->GetPosition();
    m_IsMoving = true;
    m_FirstHalfOfTheJump = true;
    m_Direction = dir;
    m_MovementDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
    m_JumpHeight = m_MoveDistance.y / 2.0f;
    m_SpeedRatiod = { m_Speed,m_Speed * m_MovementDistanceRatio * (m_MoveDistance.y / m_JumpHeight) }; // hoi

    if ((int)m_Direction >= (int)QBertSprite::DownRight)
        m_JumpHeight = m_MoveDistance.y / 2.0f;
    else
        m_JumpHeight = m_MoveDistance.y * 1.5f; // Increase jumpheight if it has to jump upwards



    MapComponent* CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();

    if (!CurrentMap->GetNextCubeIndex(m_CurrentCubeIndex, dir))
    {
        // Player jumped off the map
        m_FallingToDeath = true;
    }

}

void MoveComponent::CorrectPosition()
{

    if (m_Direction == QBertSprite::DownRightJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos + m_MoveDistance);
    }
    else if (m_Direction == QBertSprite::DownLeftJump)
    {

        m_pTransform->SetPosition(m_JumpStartPos + glm::vec2(-m_MoveDistance.x, m_MoveDistance.y));
    }
    else if (m_Direction == QBertSprite::UpLeftJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos + -m_MoveDistance);
    }
    else if (m_Direction == QBertSprite::UpRightJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos + glm::vec2(m_MoveDistance.x, -m_MoveDistance.y));
    }
}

void MoveComponent::Jump(float deltaT)
{
    glm::vec2 movement = m_pTransform->GetPosition();

    if (m_Direction == QBertSprite::DownRightJump || m_Direction == QBertSprite::UpRightJump)
        movement.x += deltaT * m_SpeedRatiod.x;
    else
        movement.x -= deltaT * m_SpeedRatiod.x;

    if (m_FirstHalfOfTheJump)
    {
        movement.y -= deltaT * m_SpeedRatiod.y;

        if (abs(movement.y - m_JumpStartPos.y) > m_JumpHeight)
            m_FirstHalfOfTheJump = false;
    }
    else
    {
        movement.y += deltaT * m_SpeedRatiod.y;
    }


    if (abs(movement.x - m_JumpStartPos.x) > m_MoveDistance.x)
    {
        // Done with jumping
        const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();
        const auto& CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();

        CorrectPosition();
        //m_pTransform->SetPosition(m_JumpStartPos + m_MoveDistance);
        // Direction - 1 = not jumping version of sprite
        int NonJumpingSprite = (int)m_Direction - 1;
        renderComp->SetSrcRect(SDL_Rect{ NonJumpingSprite * (int)renderComp->GetSpritePixelSize().x,0,(int)renderComp->GetSpritePixelSize().x,(int)renderComp->GetSpritePixelSize().y });

        CurrentMap->GetCube(m_CurrentCubeIndex)->SetActivated(true);
        m_IsMoving = false;
    }
    else
    {
        m_pTransform->SetPosition(movement.x, movement.y);
    }


}

void MoveComponent::FallToDeath(float deltaT)
{
    glm::vec2 movement = m_pTransform->GetPosition();

    if (m_Direction == QBertSprite::DownRightJump || m_Direction == QBertSprite::UpRightJump)
        movement.x += deltaT * m_SpeedRatiod.x;
    else
        movement.x -= deltaT * m_SpeedRatiod.x;

    if (m_FirstHalfOfTheJump)
    {
        movement.y -= deltaT * m_SpeedRatiod.y;

        if (abs(movement.y - m_JumpStartPos.y) > m_JumpHeight)
            m_FirstHalfOfTheJump = false;
    }
    else
    {
        movement.y += deltaT * m_SpeedRatiod.y;
    }

    if (movement.y > WINDOW_HEIGHT)
    {
        // Done with jumping
        const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();

        // Direction - 1 = not jumping version of sprite
        int NonJumpingSprite = (int)m_Direction - 1;
        renderComp->SetSrcRect(SDL_Rect{ NonJumpingSprite * (int)renderComp->GetSpritePixelSize().x,0,(int)renderComp->GetSpritePixelSize().x,(int)renderComp->GetSpritePixelSize().y });

        m_pGameObject->GetComponent<HealthComponent>()->Die();
        m_IsMoving = false;
        m_FallingToDeath = false;
    }
    else
    {
        m_pTransform->SetPosition(movement.x, movement.y);
    }

}

void MoveComponent::Update(float deltaT)
{
    if (m_FallingToDeath)
        FallToDeath(deltaT);
    else if (m_IsMoving)
        Jump(deltaT);

}



bool MoveComponent::IsMoving() const
{
    return m_IsMoving;
}