#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Subject.h"
#include "TransformComponent.h"
#include "ControlComponent.h"

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
	m_Dead = true;

	if ((int)m_Lives <= 0)
		return;

	m_Lives--;

	if (m_pGameObject->HasTag(dae::Tag::Player))
		m_pGameObject->Notify(dae::Event::ActorDied);
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