#include "MiniginPCH.h"
#include <iostream>
#include "PauseCommand.h"

#include "Minigin.h"
#include "AudioClasses.h"
#include "EnemyManager.h"
#include "CollisionManager.h"

void dae::PauseCommand::Execute(const int&)
{
	if (Minigin::GetInstance().GetGameOver())
	{
		Minigin::GetInstance().ClearGame();
	}
	else
	{
		bool paused = !Minigin::GetInstance().GetPaused();
		Minigin::GetInstance().SetPaused(paused);
		ServiceLocator::GetSoundSystem().TogglePause();
		SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("PauseDisplay")->SetEnabled(paused);
	}

	
	//InputManager::GetInstance().LockInput(!InputManager::GetInstance().GetInputLocked());
}

void dae::PauseCommand::Release(const int&)
{

}

void dae::PauseCommand::Undo()
{

}