#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "TextComponent.h"

dae::ScoreComponent::ScoreComponent(const unsigned int& score)
	:m_Score{ score }
{}

void dae::ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
	m_pGameObject->Notify(Event::ScoreSet);
}


void dae::ScoreComponent::Update(float)
{
}

const unsigned int& dae::ScoreComponent::GetScore() const
{
	return m_Score;
}

void dae::ScoreComponent::IncreaseScore(Event e)
{
	m_Score += int(e);
	m_pGameObject->Notify(e);
}
