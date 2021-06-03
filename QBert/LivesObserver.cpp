#include "MiniginPCH.h"
#include "LivesObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TimerComponent.h"
#include "ControlComponent.h"
#include "TransformComponent.h"
#include "MoveComponent.h"
void dae::LivesObserver::OnNotify(const GameObject* actor, Event event)
{
	if (event == Event::ActorDied)
		if (actor->GetComponent<ControlComponent>())
			Unlock(actor);
}

void dae::LivesObserver::Unlock(const GameObject* actor)
{
	std::shared_ptr<SceneObject> livesDisplay = nullptr;

	if (actor->HasTag(dae::Tag::Player1))
	{
		livesDisplay = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("LivesDisplayPlayer1");
		TransformComponent* transformComp = actor->GetComponent<TransformComponent>();
		if (transformComp)
		{
			const auto& playerSpawn = actor->GetComponent<ControlComponent>()->GetPlayerSpawn();
			transformComp->SetPosition(playerSpawn.x, playerSpawn.y);
		}
		else
		{
			std::cout << "WARNING: transformComp not found on player after player death" << std::endl;
		}
	}


	if (actor->HasTag(dae::Tag::Player2))
	{
		livesDisplay = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("LivesDisplayPlayer2");
		TransformComponent* transformComp = actor->GetComponent<TransformComponent>();
		if (transformComp)
		{
			const auto& playerSpawn = actor->GetComponent<ControlComponent>()->GetPlayerSpawn();
			transformComp->SetPosition(playerSpawn.x, playerSpawn.y);
		}
		else
		{
			std::cout << "WARNING: transformComp not found on player after player death" << std::endl;
		}
	}

	auto moveComp = actor->GetComponent<MoveComponent>();
	if (moveComp)
	{
		moveComp->SetCurrentCubeIndex(moveComp->GetStartCubeIndex());
	}
	else
	{
		std::cout << "WARNING: moveComp not found on player after player death" << std::endl;
	}

	if (!livesDisplay) {
		std::cout << "WARNING: livesDisplay not found after player death" << std::endl;
		return;
	}
	auto textComp = std::static_pointer_cast<GameObject>(livesDisplay).get()->GetComponent<TextComponent>();

	if (!textComp) {
		std::cout << "WARNING: TextComp of LiveCounter not found after player death" << std::endl;
		return;
	}

	auto healthComp = actor->GetComponent<HealthComponent>();

	if (!healthComp) {
		std::cout << "WARNING: HealthComp of Player not found after player death" << std::endl;
		return;
	}
	textComp->SetText("Remaining lives: " + std::to_string(healthComp->GetLives()));


	auto playerDiedPopup = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("PlayerDiedPopup");

	if (!playerDiedPopup) {
		std::cout << "WARNING: playerDiedPopup not found after player death" << std::endl;
		return;
	}

	auto timerComponent = std::static_pointer_cast<GameObject>(playerDiedPopup).get()->GetComponent<TimerComponent>();

	if (!timerComponent) {
		std::cout << "WARNING: timerComponent of playerDiedPopup not found after player death" << std::endl;
		return;
	}

	timerComponent->SetTimerEnabled(3.0f);

}