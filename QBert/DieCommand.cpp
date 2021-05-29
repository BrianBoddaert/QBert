#include "MiniginPCH.h"
#include <iostream>
#include "DieCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "HealthComponent.h"

void dae::DieCommand::Execute(const int&)
{
	std::cout << "Die" << std::endl;
	auto pPlayerActor = SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
	pPlayerActor.get()->GetComponent<HealthComponent>()->Die();
}

void dae::DieCommand::Release(const int&)
{

}


void dae::DieCommand::Undo()
{

}

