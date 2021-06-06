#include "MiniginPCH.h"
#include "MoveComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include <math.h>
#include "SceneManager.h"
#include "Scene.h"
#include "MapComponent.h"
#include "HealthComponent.h"
#include "AudioClasses.h"
using namespace dae;


MoveComponent::MoveComponent(int currentCubeIndex)
    :BaseMoveComponent{ currentCubeIndex }
{}

void MoveComponent::ActivateJump(const DirectionSprite& dir)
{
    if (m_IsMoving)
        return;


    if (m_pGameObject->HasTag(dae::Tag::Coily))
    {
        if (m_JumpCooldownTimer >= m_JumpCooldown)
            m_JumpCooldownTimer = 0.0f;
        else
            return;

    }

    ServiceLocator::GetSoundSystem().QueueSound(EffectId::Jump, 0.1f);

    const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();
    m_pTransform = m_pGameObject->GetComponent<dae::TransformComponent>();
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

    if ((int)m_Direction >= (int)DirectionSprite::DownRight)
        m_JumpHeight = m_MoveDistance.y / 2.0f;
    else
        m_JumpHeight = m_MoveDistance.y * 1.5f; // Increase jumpheight if it has to jump upwards

    MapComponent* CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();
    
    bool isOnMap = CurrentMap->SetNextCubeIndexAndCheckIfItsOnACube(m_CurrentCubeIndex, dir);

    if (!isOnMap)
    {
        // Player jumped off the map
        if (CurrentMap->DoesCubeHaveDiscNextToIt(m_CurrentCubeIndex))
        {
            m_JumpingOnDisc = true;
        }
        else
        {
            ServiceLocator::GetSoundSystem().QueueSound(EffectId::Fall, 0.1f);
            m_FallingToDeath = true;
        }

    }




}

void MoveComponent::CorrectPosition()
{
    if (m_Direction == DirectionSprite::DownRightJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x + m_MoveDistance.x, m_JumpStartPos.y + m_MoveDistance.y, m_JumpStartPos.z + 1);
    }
    else if (m_Direction == DirectionSprite::DownLeftJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x -m_MoveDistance.x, m_JumpStartPos.y + m_MoveDistance.y, m_JumpStartPos.z + 1);
    }
    else if (m_Direction == DirectionSprite::UpLeftJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x - m_MoveDistance.x, m_JumpStartPos.y - m_MoveDistance.y, m_JumpStartPos.z -1);
    }
    else if (m_Direction == DirectionSprite::UpRightJump)
    {
        m_pTransform->SetPosition(m_JumpStartPos.x + m_MoveDistance.x, m_JumpStartPos.y - m_MoveDistance.y, m_JumpStartPos.z - 1);
    }
}

void MoveComponent::Jump(float deltaT)
{
    dae::Vector2 movement = m_pTransform->GetPosition();

    if (m_Direction == DirectionSprite::DownRightJump || m_Direction == DirectionSprite::UpRightJump)
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
        int nonJumpingSprite = (int)m_Direction - 1;

        const auto& currentSrcRect = renderComp->GetSrcRect();
        const auto& spritePixelSize = renderComp->GetSpritePixelSize();
        renderComp->SetSrcRect(SDL_Rect{ nonJumpingSprite * (int)spritePixelSize.x,currentSrcRect.y,(int)spritePixelSize.x,(int)spritePixelSize.y });

        if (m_pGameObject->HasTag(dae::Tag::Player))
        CurrentMap->GetCube(m_CurrentCubeIndex)->SetActivated(true);
        if (m_pGameObject->HasTag(dae::Tag::SlickSam))
            CurrentMap->GetCube(m_CurrentCubeIndex)->SetActivated(false);
        m_IsMoving = false;
    }
    else
    {
        m_pTransform->SetPosition(movement);
    }


}

void MoveComponent::FallToDeath(float deltaT)
{
    dae::Vector2 movement = m_pTransform->GetPosition();

    if (m_Direction == DirectionSprite::DownRightJump || m_Direction == DirectionSprite::UpRightJump)
        movement.x += deltaT * m_SpeedRatiod.x;
    else
        movement.x -= deltaT * m_SpeedRatiod.x;

    if (m_FirstHalfOfTheJump)
    {
        movement.y -= deltaT * m_SpeedRatiod.y;

        if (abs(movement.y - m_JumpStartPos.y) > m_JumpHeight)
        {
            m_FirstHalfOfTheJump = false;
            if (m_Direction == DirectionSprite::DownRightJump)
            {
                m_pTransform->SetPosition(movement.x, movement.y, 7);
            }
            else if (m_Direction == DirectionSprite::DownLeftJump)
            {
                m_pTransform->SetPosition(movement.x, movement.y, 7);
            }
            else if (m_Direction == DirectionSprite::UpLeftJump)
            {
                m_pTransform->SetPosition(movement.x, movement.y, -1);
            }
            else if (m_Direction == DirectionSprite::UpRightJump)
            {
                m_pTransform->SetPosition(movement.x, movement.y, -1);
            }
        }
       
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
    else if (m_JumpingOnDisc)
        JumpOnDisc(deltaT);
    else if (m_IsMoving)
        Jump(deltaT);
    else if (m_pGameObject->HasTag(dae::Tag::Coily))
        m_JumpCooldownTimer += deltaT;
}


void MoveComponent::JumpOnDisc(float deltaT)
{
    const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

    if (!m_pDiscTransform)
    {
        if (m_IsOnDisc)
        {
            dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->MovePlayerToSpawn(m_pGameObject->GetName());
            m_CurrentCubeIndex = 0;
            m_IsOnDisc = false;
            m_JumpingOnDisc = false;
            m_FallingToDeath = false;
            m_IsMoving = false;
        }
        else
        {

            Vector3 pos = transform->GetPosition();

            const float moveDistRatio = (m_MoveDistance.y / m_MoveDistance.x);

            float jumpHeight = m_MoveDistance.y / 2.0f;

            const Vector2 speed = { m_Speed,m_Speed * moveDistRatio * (m_MoveDistance.y / jumpHeight) };

            if (m_Direction == dae::DirectionSprite::DownRightJump || m_Direction == dae::DirectionSprite::UpRightJump)
                pos.x += deltaT * speed.x;
            else
                pos.x -= deltaT * speed.x;

            jumpHeight = m_MoveDistance.y * 1.5f;

            if (m_FirstHalfOfTheJump)
            {
                pos.y -= deltaT * speed.y;

                if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
                    m_FirstHalfOfTheJump = false;
            }
            else pos.y += deltaT * speed.y;

            if (pos.y > 720)
            {
                m_IsMoving = false;
                m_FallingToDeath = false;
            }
            else transform->SetPosition(Vector3(pos.x, pos.y, 0));
        }

    }
    else
    {
        m_IsOnDisc = true;
        Vector3 newPos = m_pDiscTransform->GetPosition();
        newPos.x += GAMESCALE * 1.f;
        newPos.y -= GAMESCALE * 13.f;
        transform->SetPosition(newPos);
    }


}