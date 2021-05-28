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
    : m_Speed{ 25 }
    , m_IsMoving{ false }
    , m_CurrentCubeIndex{0 }
    , m_FallingToDeath{false}
    , m_Direction{ QBertSprite::DownLeft}
{
  const glm::vec2& cubeOffset = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetCubeOffset();
  m_MoveDistance = cubeOffset;
 
}

void MoveComponent::ActivateJump(const QBertSprite& dir)
{
   const auto& renderComp =  m_pGameObject->GetComponent<RenderComponent>();
   const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

   renderComp->SetSrcRect(SDL_Rect{ (int)dir * (int)renderComp->GetSpritePixelSize().x,0,(int)renderComp->GetSpritePixelSize().x,(int)renderComp->GetSpritePixelSize().y });
  
   m_JumpStartPos = transform->GetPosition();
   m_IsMoving = true;
   m_FirstHalfOfTheJump = true;
   m_Direction = dir;

   const auto& CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();

   if (!CurrentMap->GetNextCubeIndex(m_CurrentCubeIndex, dir))
   {
       // Player jumped off the map
       m_FallingToDeath = true;
   }

}

void MoveComponent::Jump(float deltaT)
{
    const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
    glm::vec2 pos = transform->GetPosition();

    const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
    float jumpHeight = m_MoveDistance.y / 2.0f;
    const glm::vec2 speed = { m_Speed,m_Speed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };

    if (m_Direction == QBertSprite::DownRightJump || m_Direction == QBertSprite::UpRightJump)
      pos.x += deltaT * speed.x;
    else
      pos.x -= deltaT * speed.x;

    if ((int)m_Direction >= (int)QBertSprite::DownRight)
        jumpHeight = m_MoveDistance.y / 2.0f;
    else
        jumpHeight = m_MoveDistance.y * 1.5f;

    if (m_FirstHalfOfTheJump)
    {
        pos.y -= deltaT * speed.y;

        if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
            m_FirstHalfOfTheJump = false;
    }
    else
    {
        pos.y += deltaT * speed.y;
    }


    if (abs(pos.x - m_JumpStartPos.x) > m_MoveDistance.x)
    {
        // Done with jumping
        const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();
        const auto& CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();

        // Direction - 1 = not jumping version of sprite
        int NonJumpingSprite = (int)m_Direction - 1;
        renderComp->SetSrcRect(SDL_Rect{ NonJumpingSprite * (int)renderComp->GetSpritePixelSize().x,0,(int)renderComp->GetSpritePixelSize().x,(int)renderComp->GetSpritePixelSize().y });
        CurrentMap->GetCube(m_CurrentCubeIndex)->Color();
        m_IsMoving = false;
    }

    transform->SetPosition(pos.x, pos.y);
}

void MoveComponent::FallToDeath (float deltaT)
{
    const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
    glm::vec2 pos = transform->GetPosition();

    const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
    float jumpHeight = m_MoveDistance.y / 2.0f;
    const glm::vec2 speed = { m_Speed,m_Speed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };

    if (m_Direction == QBertSprite::DownRightJump || m_Direction == QBertSprite::UpRightJump)
        pos.x += deltaT * speed.x;
    else
        pos.x -= deltaT * speed.x;

    if ((int)m_Direction >= (int)QBertSprite::DownRight)
        jumpHeight = m_MoveDistance.y / 2.0f;
    else
        jumpHeight = m_MoveDistance.y * 1.5f;

    if (m_FirstHalfOfTheJump)
    {
        pos.y -= deltaT * speed.y;

        if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
            m_FirstHalfOfTheJump = false;
    }
    else
    {
        pos.y += deltaT * speed.y;
    }

    if (pos.y > WINDOW_HEIGHT)
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
        transform->SetPosition(pos.x, pos.y);
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