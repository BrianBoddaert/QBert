#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "HealthComponent.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "MapComponent.h"

void CollisionManager::CollisionEffect(std::shared_ptr<dae::GameObject> player, std::shared_ptr<dae::GameObject> collider)
{
	if (collider->HasTag(dae::Tag::Coily))
	{
		player->GetComponent<HealthComponent>()->Die();
	}
	else if (collider->HasTag(dae::Tag::Disc))
	{
		auto moveComp = player->GetComponent<MoveComponent>();
		if (!moveComp->GetJumpingOnDisc())
			return;

		moveComp->SetDiscTransform(collider->GetComponent<dae::TransformComponent>());
		dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetDiscByGameObject(collider)->SetMoving(true);
	}
}