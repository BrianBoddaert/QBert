#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "TextComponent.h"

ScoreComponent::ScoreComponent(const unsigned int& score)
	:m_Score{ score }
{}

void ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
	m_pGameObject->Notify(dae::Event::ScoreSet);
}


void ScoreComponent::Update(float)
{
}

const unsigned int& ScoreComponent::GetScore() const
{
	return m_Score;
}

void ScoreComponent::IncreaseScore(dae::Event e)
{
	m_Score += int(e);
	m_pGameObject->Notify(e);
}
