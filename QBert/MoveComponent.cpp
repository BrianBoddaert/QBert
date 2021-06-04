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


MoveComponent::MoveComponent(int currentCubeIndex)
    : m_Speed{ 50.0f }
    , m_IsMoving{ false }
    , m_StartCubeIndex{ currentCubeIndex }
    , m_CurrentCubeIndex{ currentCubeIndex }
    , m_FallingToDeath{ false }
    , m_Direction{ QBertSprite::DownLeft }
{
    const dae::Vector2& cubeOffset = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetCubeOffset();
    m_MoveDistance = cubeOffset * GAMESCALE;
}

void MoveComponent::ActivateJump(const QBertSprite& dir)
{
    if (m_IsMoving)
        return;

    const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();
    m_pTransform = m_pGameObject->GetComponent<TransformComponent>();
    const auto& currentSrcRect = renderComp->GetSrcRect();
    const auto& spritePixelSize = renderComp->GetSpritePixelSize();
    renderComp->SetSrcRect(SDL_Rect{ (int)dir * (int)spritePixelSize.x,currentSrcRect.y,(int)spritePixelSize.x,(int)spritePixelSize.y });

    m_JumpStartPos = m_pTransform->GetPosition();
    m_IsMoving = true;
    m_FirstHalfOfTheJump = true;
    m_Direction = dir;
    m_MovementDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
    m_JumpHeight = m_MoveDistance.y / 2.0f;
    m_SpeedRatiod = { m_Speed,m_Speed * m_MovementDistanceRatio * (m_MoveDistance.y / m_JumpHeight) };

    if ((int)m_Direction >= (int)QBertSprite::DownRight)
        m_JumpHeight = m_MoveDistance.y / 2.0f;
    else
        m_JumpHeight = m_MoveDistance.y * 1.5f; // Increase jumpheight if it has to jump upwards

    MapComponent* CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();
    
    bool isOnMap = CurrentMap->SetNextCubeIndexAndCheckIfItsOnACube(m_CurrentCubeIndex, dir);

    if (!isOnMap)
    {
        // Player jumped off the map
        m_FallingToDeath = true;
    }

}

void MoveComponent::CorrectPosition()
{

    if (m_Direction == QBertSprite::DownRightJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x + m_MoveDistance.x, m_JumpStartPos.y + m_MoveDistance.y, m_JumpStartPos.z + 1);
    }
    else if (m_Direction == QBertSprite::DownLeftJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x -m_MoveDistance.x, m_JumpStartPos.y + m_MoveDistance.y, m_JumpStartPos.z + 1);
    }
    else if (m_Direction == QBertSprite::UpLeftJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x - m_MoveDistance.x, m_JumpStartPos.y - m_MoveDistance.y, m_JumpStartPos.z -1);
    }
    else if (m_Direction == QBertSprite::UpRightJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x + m_MoveDistance.x, m_JumpStartPos.y - m_MoveDistance.y, m_JumpStartPos.z - 1);
    }
}

void MoveComponent::Jump(float deltaT)
{
    dae::Vector3 movement = m_pTransform->GetPosition();

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

        const auto& currentSrcRect = renderComp->GetSrcRect();
        const auto& spritePixelSize = renderComp->GetSpritePixelSize();
        renderComp->SetSrcRect(SDL_Rect{ NonJumpingSprite * (int)spritePixelSize.x,currentSrcRect.y,(int)spritePixelSize.x,(int)spritePixelSize.y });

        if (m_pGameObject->HasTag(dae::Tag::Player))
        CurrentMap->GetCube(m_CurrentCubeIndex)->SetActivated(true);

        m_IsMoving = false;
    }
    else
    {
        m_pTransform->SetPosition(movement);
    }


}

void MoveComponent::FallToDeath(float deltaT)
{
    dae::Vector3 movement = m_pTransform->GetPosition();

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

        const auto& currentSrcRect = renderComp->GetSrcRect();
        const auto& spritePixelSize = renderComp->GetSpritePixelSize();
        renderComp->SetSrcRect(SDL_Rect{ NonJumpingSprite * (int)spritePixelSize.x,currentSrcRect.y,(int)spritePixelSize.x,(int)spritePixelSize.y });

        m_pGameObject->GetComponent<HealthComponent>()->Die();
        m_IsMoving = false;
        m_FallingToDeath = false;
    }
    else
    {
        m_pTransform->SetPosition(movement);
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