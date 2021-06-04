#include "MiniginPCH.h"
#include "LevelCompleteObserver.h"

#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "MapComponent.h"
#include "ControlComponent.h"
#include "TransformComponent.h"
#include "MoveComponent.h"
#include "InputManager.h"
#include "AudioClasses.h"
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
	ServiceLocator::GetSoundSystem().QueueSound(EffectId::Victory, 0.3f);
	InputManager::GetInstance().LockInput(true);

}