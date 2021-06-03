#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Subject.h"
#include "TransformComponent.h"
#include "ControlComponent.h"

dae::HealthComponent::HealthComponent(const unsigned int& health)
	:m_Lives{ health }
	, m_Dead{ false }
{}

void dae::HealthComponent::SetLives(const unsigned int& health)
{
	m_Lives = health;
}

void dae::HealthComponent::Die()
{
	m_Dead = true;

	if ((int)m_Lives <= 0)
		return;

	m_Lives--;

	if (m_pGameObject->GetComponent<ControlComponent>())
		m_pGameObject->Notify(Event::ActorDied);
}

void dae::HealthComponent::Respawn()
{
	m_Dead = false;
}

void dae::HealthComponent::Update(float)
{
	//if (m_pTextComponent)
	//	m_pTextComponent->SetText("Remaining lives: " + std::to_string(m_Lives));
}

const unsigned int& dae::HealthComponent::GetLives() const
{
	return m_Lives;
}

bool dae::HealthComponent::IsAlive() const
{
	return (!m_Dead);
}