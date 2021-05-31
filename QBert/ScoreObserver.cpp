#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

void dae::ScoreObserver::OnNotify(const GameObject* actor, Event event)
{
	if (event == Event::CatchSlickOrSam || event == Event::CoilyHitWithFlyingDisc || event == Event::ColorChanged || event == Event::DiscLeftAtEndOfTheStage || event == Event::ScoreSet)
		Unlock(actor);
}

void dae::ScoreObserver::Unlock(const GameObject* actor)
{

	std::shared_ptr<SceneObject> pointsDisplay = nullptr;

	if (actor->GetName() == "Q*Bert1")
		pointsDisplay = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplayPlayer1");

	if (actor->GetName() == "Q*Bert2")
		pointsDisplay = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplayPlayer2");

	if (!pointsDisplay) {
		std::cout << "WARNING: pointsDisplay not found after player death" << std::endl;
		return;
	}
	auto textComp = std::static_pointer_cast<GameObject>(pointsDisplay).get()->GetComponent<TextComponent>();

	if (!textComp) {
		std::cout << "WARNING: TextComp of pointsDisplay not found after player death" << std::endl;
		return;
	}

	auto scoreComp = actor->GetComponent<ScoreComponent>();

	if (!scoreComp) {
		std::cout << "WARNING: ScoreComp not found after player death" << std::endl;
		return;
	}
	textComp->SetText("Score: " + std::to_string(scoreComp->GetScore()));


}