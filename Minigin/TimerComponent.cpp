#include "MiniginPCH.h"
#include "TimerComponent.h"

dae::TimerComponent::TimerComponent()
	:m_TimerDisabled{ 0 }
	, m_TimerEnabled{ 0 }
{

}

void dae::TimerComponent::Update(float deltaT)
{

	if (m_TimerDisabled > 0.0f)
	{
		m_TimerDisabled -= deltaT;

		if (m_TimerDisabled <= 0)
			m_pGameObject->SetEnabled(true);

	}

	if (m_TimerEnabled > 0.0f)
	{
		m_TimerEnabled -= deltaT;

		if (m_TimerEnabled <= 0)
			m_pGameObject->SetEnabled(false);

	}


}
void dae::TimerComponent::SetTimerDisabled(const float& time)
{
	m_pGameObject->SetEnabled(false);
	m_TimerDisabled = time;
}
void dae::TimerComponent::SetTimerEnabled(const float& time)
{
	m_pGameObject->SetEnabled(true);
	m_TimerEnabled = time;
}