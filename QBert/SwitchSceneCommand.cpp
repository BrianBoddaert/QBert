#include "MiniginPCH.h"
#include "SwitchSceneCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "ControlComponent.h"
#include "Minigin.h"

#include "EnemyManager.h"
#include "CollisionManager.h"

void SwitchSceneCommand::Execute(const int&)
{
	if (Minigin::GetInstance().GetPaused())
		return;

	EnemyManager::GetInstance().Reset();
	CollisionManager::GetInstance().ClearColliders();

	auto& sceneManager = dae::SceneManager::GetInstance();
	sceneManager.GetCurrentScene()->SetGameModeToNext();
	sceneManager.RemoveCurrentScene();
	dae::GameMode gamemode = sceneManager.GetCurrentScene()->GetGameMode();

	if (gamemode == dae::GameMode::Versus)
		gamemode = dae::GameMode::SinglePlayer;
	else
		gamemode = dae::GameMode((int)gamemode + 1);

	Minigin::GetInstance().LoadSceneByGameMode(gamemode);

}

void SwitchSceneCommand::Release(const int&)
{}

void SwitchSceneCommand::Undo()
{

}


