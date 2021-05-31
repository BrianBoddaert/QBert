#include "Subject.h"


dae::Subject::Subject()
	:m_ObserverCount{ 0 }, m_pObservers{nullptr}
{}

dae::Subject::~Subject()
{
	for (size_t i = 0; i < m_ObserverCount; i++)
	{
		delete m_pObservers[i];
	}
}

void dae::Subject::AddObserver(Observer* observer)
{
	m_pObservers[m_ObserverCount] = observer;
	m_ObserverCount++;
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	for (size_t i = 0; i < m_ObserverCount; i++)
	{
		if (m_pObservers[i] == observer)
		{
			delete m_pObservers[i];
			m_pObservers[i] = nullptr;
		}
	}
}

void dae::Subject::Notify(const GameObject* actor, Event event)
{
	for (size_t i = 0; i < m_ObserverCount; i++)
		m_pObservers[i]->OnNotify(actor, event);
	
}