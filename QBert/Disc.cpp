#include "MiniginPCH.h"
#include "Disc.h"
#include "SceneManager.h"
#include <cmath>
#include "MoveComponent.h"
#include "RenderComponent.h"
#include "MapComponent.h"
#include "AudioClasses.h"

using namespace dae;

Disc::Disc(const Vector3& finalPos,const Vector2& srcRect)
	:m_FinalPos{ finalPos }
{
	m_pGameObject = std::make_shared<GameObject>(("Disc"));
	m_FinalPos.y -= srcRect.y;
	m_FinalPos.x += srcRect.x/2; 
}

void Disc::Update(float deltaT)
{
	m_pGameObject->Update(deltaT);
	Elevate(deltaT);

	if (m_AnimTimer > 0)
	{
		m_AnimTimer -= deltaT;
	}
	else
	{
		//0,352 16x16
		auto srcrect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();
		if (srcrect.x == 48)
			srcrect.x = 0;
		else
			srcrect.x += srcrect.w;

		m_pGameObject->GetComponent<RenderComponent>()->SetSrcRect(srcrect);
		m_AnimTimer = m_AnimTime;
	}
}

void Disc::Render() const
{
	m_pGameObject->Render();
}

void Disc::Elevate(float deltaT)
{
	if (m_Direction == Vector3{ 0,0,0 } && m_Moving)
	{
		auto dist = m_FinalPos - m_pGameObject->GetComponent<TransformComponent>()->GetPosition();

		auto length = sqrt((dist.x * dist.x) + (dist.y * dist.y));

		m_Direction.x = dist.x / length;
		m_Direction.y = dist.y / length;
		ServiceLocator::GetSoundSystem().QueueSound(EffectId::Lift, 0.3f);
	}

	else if (m_Moving)
	{
		m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();

		Vector3 pos = m_pTransformComponent->GetPosition();

		if (abs(pos.x - m_FinalPos.x) > 2)
		{
			auto newPos = pos + m_Direction * deltaT * m_Speed;
			m_pTransformComponent->SetPosition(newPos);

		}
		else // Reached the top
		{
			m_pTransformComponent->SetPosition(Vector3{ m_FinalPos.x, m_FinalPos.y, 0 });
			auto players = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayers();
			for (size_t i = 0; i < players.size(); i++)
			{
				auto moveComp = players[i]->GetComponent<MoveComponent>();
				if (moveComp->GetDiscGameObject() == m_pGameObject)
					moveComp->SetDiscGameObject(nullptr);
			}
			m_Moving = false;
			m_MarkedForDelete = true;
		}
	}
}

