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


	if (m_pGameObject->HasTag(dae::Tag::Coily))
	{
		DirectionSprite currentSprite = FindDirectionToClosestPlayer();

		if (currentSprite == DirectionSprite::DownRight)
			return;

		moveComp->ActivateJump(currentSprite);
	}
	if (m_pGameObject->HasTag(dae::Tag::SlickSam))
	{


}

dae::DirectionSprite PathFindingComponent::FindDirectionToClosestPlayer() const
{
	std::shared_ptr<GameObject> closestPlayer = FindClosestPlayer();

	if (!closestPlayer)
		return DirectionSprite::DownRight;

	const dae::Vector3& currentPos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	const dae::Vector3& playerPos = closestPlayer->GetComponent<TransformComponent>()->GetPosition();

	if (playerPos.x <= currentPos.x && playerPos.y <= currentPos.y)
		return DirectionSprite::UpLeftJump;
	else if (playerPos.x >= currentPos.x && playerPos.y <= currentPos.y)
		return DirectionSprite::UpRightJump;
	else if (playerPos.x <= currentPos.x && playerPos.y >= currentPos.y) 
		return DirectionSprite::DownLeftJump;
	else if (playerPos.x >= currentPos.x && playerPos.y >= currentPos.y) 
		return DirectionSprite::DownRightJump;

	return DirectionSprite::DownRight;
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

