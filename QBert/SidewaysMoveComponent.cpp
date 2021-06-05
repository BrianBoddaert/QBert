#include "MiniginPCH.h"
#include "SidewaysMoveComponent.h"
#include "TransformComponent.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "MapComponent.h"
#include "AudioClasses.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "Cube.h"

using namespace dae;

SidewaysMoveComponent::SidewaysMoveComponent(int tileIndex, bool side)
	: BaseMoveComponent{tileIndex}
	, m_StartSide{ side }
{}

void SidewaysMoveComponent::Update(float deltaT)
{
	if (m_FallingToDeath)
		FallToDeath(deltaT);
	else if (m_IsMoving)
		Jump(deltaT);
	else
		m_JumpCooldownTimer += deltaT;
	
	if (m_JumpCooldownTimer >= m_JumpCooldown)
	{
		m_JumpCooldownTimer = 0;
		ActivateJump(RandomizeDirection());

	}
}


void SidewaysMoveComponent::CorrectPosition() 
{
	MapComponent* CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();
	auto cube = CurrentMap->GetCube(m_CurrentCubeIndex);
	auto cubePos = cube->GetGameObject()->GetComponent<TransformComponent>()->GetPosition();
	auto srcRectCube = cube->GetGameObject()->GetComponent<RenderComponent>()->GetSrcRect();
	auto srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();

	if (m_StartSide)
	{
		m_pGameObject->GetComponent<TransformComponent>()->SetPosition(Vector2(cubePos.x + srcRect.w/2 + srcRectCube.w / 4, cubePos.y + srcRect.h/2 + srcRectCube.h / 4));
	}
	else
	{
		m_pGameObject->GetComponent<TransformComponent>()->SetPosition(Vector2(cubePos.x + srcRect.w/2 - srcRectCube.w / 4, cubePos.y + srcRect.h/2 + srcRectCube.h / 4));
	}


}

const dae::DirectionSprite SidewaysMoveComponent::RandomizeDirection() const
{
	bool upOrDown = rand() % 2;

	if (m_StartSide)
	{
		if (upOrDown)
			return DirectionSprite::DownRightJump;
		else
			return DirectionSprite::UpRightJump;
	}
	else
	{
		if (upOrDown)
			return DirectionSprite::DownLeftJump;
		else
			return DirectionSprite::UpLeftJump;

	}
}

void SidewaysMoveComponent::ActivateJump(const DirectionSprite& dir)
{

	// CAN BE IN BASE CLASS ITS THE SAME SO FAR <<<<
	m_Direction = dir;
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

	bool isOnMap = CurrentMap->SetNextCubeIndexAndCheckIfItsOnACube(m_CurrentCubeIndex, dir,true);

	if (!isOnMap)
	{
		// Player jumped off the map
		ServiceLocator::GetSoundSystem().QueueSound(EffectId::Fall, 0.1f);
		m_FallingToDeath = true;
	}

}

void SidewaysMoveComponent::Jump(float deltaT)
{

	const TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	Vector3 movement = transform->GetPosition();

	if (m_Direction == dae::DirectionSprite::DownRightJump || m_Direction == dae::DirectionSprite::DownLeftJump)
	{
		m_SpeedRatiod.x *= 1.5;
	}

	if (m_Direction == dae::DirectionSprite::UpRightJump || m_Direction == dae::DirectionSprite::DownRightJump)
		movement.x += deltaT * m_SpeedRatiod.x;
	else
		movement.x -= deltaT * m_SpeedRatiod.x;

	if (m_Direction != dae::DirectionSprite::DownRightJump && m_Direction != dae::DirectionSprite::DownLeftJump)
	{
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
	}


	if (abs(movement.x - m_JumpStartPos.x) > m_MoveDistance.x)
	{
        // Done with jumping
		const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();

		CorrectPosition();

		int nonJumpingSprite = (int)m_Direction - 1;

		const auto& currentSrcRect = renderComp->GetSrcRect();
		const auto& spritePixelSize = renderComp->GetSpritePixelSize();
		renderComp->SetSrcRect(SDL_Rect{ nonJumpingSprite * (int)spritePixelSize.x,currentSrcRect.y,(int)spritePixelSize.x,(int)spritePixelSize.y });

		m_IsMoving = false;
	}
	else
	{
		m_pTransform->SetPosition(movement);
	}
}

void SidewaysMoveComponent::FallToDeath(float deltaT)
{

	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	Vector3 movement = transform->GetPosition();

	if (m_StartSide)
		movement.x += deltaT * m_Speed;
	else
		movement.x -= deltaT * m_Speed;

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