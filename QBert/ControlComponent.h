#pragma once
#include "Component.h"
#include "MoveCommand.h"

namespace dae
{
	enum class QBertSprite
	{
		UpRight,
		UpRightJump,
		UpLeft,
		UpLeftJump,
		DownRight,
		DownRightJump,
		DownLeft,
		DownLeftJump
	};
}

class ControlComponent :
	public dae::Component
{
public:
	ControlComponent();
	void Initialize();
	void SetMoveInput(const dae::MoveInputDirections& dir, bool on);
	void Update(float) override;
	void SetPlayerSpawn(const glm::vec2& spawn) { m_PlayerSpawn = spawn; };
	const glm::vec2& GetPlayerSpawn() { return m_PlayerSpawn; };

private:
	glm::vec2 m_PlayerSpawn;
	dae::QBertSprite m_CurrentSprite;
	bool m_MoveInputsActive[long(dae::MoveInputDirections::Count)];
	bool m_Moving;

};




