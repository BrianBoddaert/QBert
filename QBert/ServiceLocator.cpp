#include "MiniginPCH.h"
#include "AudioClasses.h"

dae::SoundSystem* dae::ServiceLocator::m_pSsInstance;
dae::NullSoundSystem dae::ServiceLocator::m_NullSs;


dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	if (m_pSsInstance)
		return *m_pSsInstance;
	else
		return m_NullSs;
}

void dae::ServiceLocator::SetSoundSystem(dae::SoundSystem* ss)
{
	m_pSsInstance = ss == nullptr ? &m_NullSs : ss;
}

void dae::ServiceLocator::CleanUp()
{
	if (m_pSsInstance)
		delete m_pSsInstance;
}