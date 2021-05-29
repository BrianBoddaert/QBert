#include "MiniginPCH.h"
#include <iostream>
#include "IncreasePointsCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "ScoreComponent.h"

void dae::IncreasePointsCommand::Execute(const int&)
{
	std::cout << "PointIncrease" << std::endl;
	auto pPlayerActor = SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
	pPlayerActor.get()->GetComponent<ScoreComponent>()->IncreaseScore(Event::ColorChanged);
}

void dae::IncreasePointsCommand::Release(const int&)
{

}

void dae::IncreasePointsCommand::Undo()
{

}

