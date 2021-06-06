#include "MiniginPCH.h"
#include "BaseMoveComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MapComponent.h"

using namespace dae;

BaseMoveComponent::BaseMoveComponent(int tileIndex)
    : m_Speed{ 50.0f }
    , m_IsMoving{ false }
    , m_StartCubeIndex{ tileIndex }
    , m_CurrentCubeIndex{ tileIndex }
    , m_FallingToDeath{ false }
    , m_Direction{ dae::DirectionSprite::DownLeft }
    , m_JumpCooldownTimer{ 0 }
    , m_JumpCooldown{ 0.15f }
    , m_EnemyJumpCooldownMultiplier{ 2.0f }
    , m_JumpHeightUpMultiplier{ 1.5f }
{
    const dae::Vector2& cubeOffset = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetCubeOffset();
    m_MoveDistance = cubeOffset * GAMESCALE;
};