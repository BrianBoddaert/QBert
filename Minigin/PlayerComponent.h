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


	class PlayerComponent :
		public Component
	{
	public:
		PlayerComponent();
		void Initialize();
		void SetMoveInput(const MoveInputDirections& dir, bool on);
		void Update(float) override;
		static void SetPlayerSpawn(const glm::vec2& spawn) { m_PlayerSpawn = spawn; };
		static const glm::vec2& GetPlayerSpawn() { return m_PlayerSpawn; };

	private:
		static glm::vec2 m_PlayerSpawn;
		QBertSprite m_CurrentSprite;
		bool m_MoveInputsActive[long(MoveInputDirections::Count)];
		
	};



}

