#include "MiniginPCH.h"
#include "LevelCompleteObserver.h"

#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "MapComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "MoveComponent.h"
#include "InputManager.h"
using namespace dae;

void LevelCompleteObserver::OnNotify(const GameObject* actor, Event event)
{
	if (event == Event::ColorChanged)
		if (actor->GetComponent<MapComponent>()->LevelCompleteCheck())
			Unlock(actor);
}

void LevelCompleteObserver::Unlock(const GameObject* go)
{
	MapComponent* mapComp = go->GetComponent<MapComponent>();

	mapComp->SetLevelFinished(true);
	InputManager::GetInstance().LockInput(true);

}