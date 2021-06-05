#include "MiniginPCH.h"
#include "ControlComponent.h"
#include "MoveComponent.h"
#include "InputManager.h"

using namespace dae;

ControlComponent::ControlComponent(const dae::Vector3& spawn)
	:m_SpawnPosition{ spawn }
{

	Initialize();
}

void ControlComponent::Initialize()
{

}

void ControlComponent::SetMoveInput(const MoveInputDirections& dir, bool on)
{
	m_MoveInputsActive[int(dir)] = on;
}

void ControlComponent::Update(float)
{
	MoveComponent* moveComp = m_pGameObject->GetComponent<MoveComponent>();
	DirectionSprite currentSprite;

	if (moveComp->IsMoving() || InputManager::GetInstance().GetInputLocked())
		return;

	if (m_MoveInputsActive[(int)MoveInputDirections::Down] && m_MoveInputsActive[(int)MoveInputDirections::Left])
		currentSprite = DirectionSprite::DownLeftJump;
	else if (m_MoveInputsActive[(int)MoveInputDirections::Up] && m_MoveInputsActive[(int)MoveInputDirections::Left])
		currentSprite = DirectionSprite::UpLeftJump;
	else if (m_MoveInputsActive[(int)MoveInputDirections::Down] && m_MoveInputsActive[(int)MoveInputDirections::Right])
		currentSprite = DirectionSprite::DownRightJump;
	else if (m_MoveInputsActive[(int)MoveInputDirections::Up] && m_MoveInputsActive[(int)MoveInputDirections::Right])
		currentSprite = DirectionSprite::UpRightJump;
	else
		return;

	moveComp->ActivateJump(currentSprite);
}