#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "Structs.h"
namespace dae
{
	enum class DirectionSprite
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
	ControlComponent(const dae::Vector3& spawn);
	void Initialize();
	void SetMoveInput(const MoveInputDirections& dir, bool on);
	void Update(float) override;
	const dae::Vector3& GetSpawnPosition() const { return m_SpawnPosition; }
private:
	const dae::Vector3 m_SpawnPosition;
	bool m_MoveInputsActive[long(MoveInputDirections::Count)];
	bool m_Moving;

};




