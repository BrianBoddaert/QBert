#include "MiniginPCH.h"
#include "MoveCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "PlayerComponent.h"

void dae::MoveCommand::Execute(const int& dir)
{
	int playerIndex = m_ControllerIndex;

	if (m_ControllerIndex == -1)
		playerIndex = 0;

	auto player = SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(playerIndex);
	const auto& playerComp = player.get()->GetComponent<PlayerComponent>();
	playerComp->SetMoveInput(MoveInputDirections(dir),true);
}

void dae::MoveCommand::Release(const int& dir)
{
	int playerIndex = m_ControllerIndex;

	if (m_ControllerIndex == -1)
		playerIndex = 0;

	auto player = SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(playerIndex);
	const auto& playerComp = player.get()->GetComponent<PlayerComponent>();
	playerComp->SetMoveInput(MoveInputDirections(dir), false);
}
void dae::MoveCommand::Undo()
{

}


