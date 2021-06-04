#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "HealthComponent.h"

void CollisionManager::CollisionEffect(std::shared_ptr<dae::GameObject> player, std::shared_ptr<dae::GameObject> collider)
{
	if (collider->HasTag(dae::Tag::Coily))
	{
		player->GetComponent<HealthComponent>()->Die();
	}
}