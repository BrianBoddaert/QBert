#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

using namespace dae;

CollisionManager::CollisionManager()
{}
void CollisionManager::Update()
{

	for (size_t i = 0; i < m_pPlayers.size(); i++)
	{
		for (size_t j = 0; j < m_pColliders.size(); j++)
		{
			if (IsColliding(m_pPlayers[i], m_pColliders[j]))
			{
				CollisionEffect(m_pPlayers[i], m_pColliders[j]);
			}
		}
	}

}

void CollisionManager::CollisionEffect(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> collider)
{
	if (collider->HasTag(dae::Tag::Coily))
	{
		/*player->GetComponent<HealthComponent>()*/
	}

}

void CollisionManager::AddCollider(const std::shared_ptr<GameObject>& gameObject)
{
	if (gameObject->HasTag(dae::Tag::Player))
	{
		m_pPlayers.push_back(gameObject);
	}
	else
	{
		m_pColliders.push_back(gameObject);
	}
}

bool CollisionManager::IsColliding(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> collider)
{
	const glm::vec2 pos1 = player->GetComponent<TransformComponent>()->GetPosition();
	const glm::vec2 size1 = player->GetComponent<RenderComponent>()->GetSpritePixelSize();
	const glm::vec2 pos2 = collider->GetComponent<TransformComponent>()->GetPosition();
	const glm::vec2 size2 = collider->GetComponent<RenderComponent>()->GetSpritePixelSize();

	if (pos1.x + size1.x >= pos2.x && pos1.y + size1.y >= pos1.y && !(pos1.x > pos2.x + size2.x) && !(pos1.y > pos2.y + size2.y))
	{
		return true;
	}
	else if (pos2.x + size2.x >= pos1.x && pos2.y + size2.y >= pos2.y && !(pos2.x > pos1.x + size1.x) && !(pos2.y > pos1.y + size1.y))
	{
		return true;
	}

	return false;
}