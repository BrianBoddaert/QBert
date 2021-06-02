#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "MoveComponent.h"
#include "InputManager.h"
using namespace dae;



PlayerComponent::PlayerComponent()
	:m_CurrentSprite{ QBertSprite::DownRight }
{
	Initialize();
}

void PlayerComponent::Initialize()
{

}

void PlayerComponent::SetMoveInput(const MoveInputDirections& dir, bool on)
{
	m_MoveInputsActive[int(dir)] = on;
}

void PlayerComponent::Update(float)
{
	MoveComponent* animComp = m_pGameObject->GetComponent<MoveComponent>();
	
	if (animComp->IsMoving() || InputManager::GetInstance().GetInputLocked())
		return;

	if (m_MoveInputsActive[(int)MoveInputDirections::Down] && m_MoveInputsActive[(int)MoveInputDirections::Left])
		m_CurrentSprite = QBertSprite::DownLeftJump;
	else if (m_MoveInputsActive[(int)MoveInputDirections::Up] && m_MoveInputsActive[(int)MoveInputDirections::Left])
		m_CurrentSprite = QBertSprite::UpLeftJump;
	else if (m_MoveInputsActive[(int)MoveInputDirections::Down] && m_MoveInputsActive[(int)MoveInputDirections::Right])
		m_CurrentSprite = QBertSprite::DownRightJump;
	else if (m_MoveInputsActive[(int)MoveInputDirections::Up] && m_MoveInputsActive[(int)MoveInputDirections::Right])
		m_CurrentSprite = QBertSprite::UpRightJump;
	else
		return;

	animComp->ActivateJump(m_CurrentSprite);
}