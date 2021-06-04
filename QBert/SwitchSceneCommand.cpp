#include "MiniginPCH.h"
#include "SwitchSceneCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "ControlComponent.h"

void SwitchSceneCommand::Execute(const int&)
{
	dae::SceneManager::GetInstance().SetCurrentSceneToNext();
}

void SwitchSceneCommand::Release(const int&)
{}

void SwitchSceneCommand::Undo()
{

}


