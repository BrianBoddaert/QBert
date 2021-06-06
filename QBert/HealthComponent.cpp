#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Subject.h"
#include "TransformComponent.h"
#include "ControlComponent.h"
#include "EnemyManager.h"
#include "MoveComponent.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Minigin.h"
#include "AudioClasses.h"

HealthComponent::HealthComponent(const unsigned int& health)
	:m_Lives{ health }
	, m_Dead{ false }
{}

void HealthComponent::SetLives(const unsigned int& health)
{
	m_Lives = health;
}

void HealthComponent::Die()
{


	if ((int)m_Lives <= 0)
	{
		m_Dead = true;
		m_pGameObject->GetComponent<MoveComponent>()->SetIsMoving(false);
		dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("GameOverDisplay")->SetEnabled(true);
		dae::SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("PressToRestart")->SetEnabled(true);

		dae::ServiceLocator::GetSoundSystem().TogglePause();
		Minigin::GetInstance().SetGameOver(true);
	}
	else
	{
		m_Lives--;
	}

	if (m_pGameObject->HasTag(dae::Tag::Player))
	{
		m_pGameObject->GetComponent<MoveComponent>()->SetIsMoving(false);
		m_pGameObject->Notify(dae::Event::ActorDied);
	}
	else
	{
		EnemyManager::GetInstance().RemoveEnemyByName(m_pGameObject->GetName());
	}
}

void HealthComponent::Respawn()
{
	m_Dead = false;
}

void HealthComponent::Update(float)
{
	//if (m_pTextComponent)
	//	m_pTextComponent->SetText("Remaining lives: " + std::to_string(m_Lives));
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}

bool HealthComponent::IsAlive() const
{
	return (!m_Dead);
}