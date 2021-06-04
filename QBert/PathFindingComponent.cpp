#include "MiniginPCH.h"
#include "PathFindingComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TransformComponent.h"
#include "MoveComponent.h"
#include "InputManager.h"
#include <cmath>

using namespace dae;

PathFindingComponent::PathFindingComponent()
{

}

void PathFindingComponent::Update(float)
{
	MoveComponent* moveComp = m_pGameObject->GetComponent<MoveComponent>();

	if (moveComp->IsMoving() || InputManager::GetInstance().GetInputLocked())
		return;
	QBertSprite currentSprite = FindDirectionToClosestPlayer();

	if (currentSprite == QBertSprite::DownRight)
		return;

	moveComp->ActivateJump(currentSprite);


}

dae::QBertSprite PathFindingComponent::FindDirectionToClosestPlayer() const
{
	std::shared_ptr<GameObject> closestPlayer = FindClosestPlayer();

	if (!closestPlayer)
		return QBertSprite::DownRight;

	const dae::Vector3& currentPos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	const dae::Vector3& playerPos = closestPlayer->GetComponent<TransformComponent>()->GetPosition();

	if (playerPos.x <= currentPos.x && playerPos.y <= currentPos.y)
		return QBertSprite::UpLeftJump;
	else if (playerPos.x >= currentPos.x && playerPos.y <= currentPos.y)
		return QBertSprite::UpRightJump;
	else if (playerPos.x <= currentPos.x && playerPos.y >= currentPos.y) 
		return QBertSprite::DownLeftJump;
	else if (playerPos.x >= currentPos.x && playerPos.y >= currentPos.y) 
		return QBertSprite::DownRightJump;

	return QBertSprite::DownRight;
}

const std::shared_ptr<dae::GameObject> PathFindingComponent::FindClosestPlayer() const
{
	const dae::Vector3& currentPos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();

	const std::vector< std::shared_ptr<GameObject>>& players = SceneManager::GetInstance().GetCurrentScene()->GetPlayers();

	float distanceToClosestPlayer = FLT_MAX;
	std::shared_ptr<GameObject> closestPlayer = nullptr;

	if (players.size() == 0)
		return nullptr;

	for (size_t i = 0; i < players.size(); i++)
	{
		if (!players[i]->HasTag(dae::Tag::Player))
			continue;

		const  dae::Vector3& playerPos = players[i]->GetComponent<TransformComponent>()->GetPosition();
		float distanceToPlayer = GetDistanceBetween(playerPos, currentPos);
		if (distanceToPlayer < distanceToClosestPlayer)
		{
			distanceToClosestPlayer = distanceToPlayer;
			closestPlayer = players[i];
		}
	}

	return closestPlayer;
}

float PathFindingComponent::GetDistanceBetween(const dae::Vector3 & a, const dae::Vector3 & b) const
{
	dae::Vector3 c = a - b;
	c.x = abs(c.x);
	c.y = abs(c.y);
	return sqrt(c.x * c.x + c.y * c.y);
	
}

