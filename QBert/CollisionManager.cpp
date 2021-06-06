#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "HealthComponent.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "MapComponent.h"
#include "EnemyManager.h"
#include "ScoreComponent.h"

void CollisionManager::CollisionEffect(std::shared_ptr<dae::GameObject> player, std::shared_ptr<dae::GameObject> collider)
{
	if (player->GetComponent<MoveComponent>()->GetIsOnDisc() || player->GetComponent<MoveComponent>()->IsFallingToDeath())
		return;

	auto colliderMoveComp = collider->GetComponent<MoveComponent>();
	if (colliderMoveComp)
		if (colliderMoveComp->IsFallingToDeath())
			return;

	if (collider->HasTag(dae::Tag::Coily) || collider->HasTag(dae::Tag::WrongWay))
	{
		player->GetComponent<HealthComponent>()->Die();

		//EnemyManager::GetInstance().Reset();
	}
	else if (collider->HasTag(dae::Tag::SlickSam))
	{
		collider->GetComponent<HealthComponent>()->Die();
		auto scoreComponent = player->GetComponent<ScoreComponent>();
		scoreComponent->IncreaseScore(dae::Event::CatchSlickOrSam);
		
	}
	else if (collider->HasTag(dae::Tag::Disc))
	{
		auto moveComp = player->GetComponent<MoveComponent>();
		if (!moveComp->GetJumpingOnDisc())
			return;

		moveComp->SetDiscGameObject(collider);
		dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetDiscByGameObject(collider)->SetMoving(true);
	}
}