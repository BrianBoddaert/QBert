#pragma once
#include "Command.h"
namespace dae
{
	enum class MoveInputDirections
	{
		Up,
		Right,
		Down,
		Left,
		Count
	};

	class MoveCommand : public Command
	{
	public:
		MoveCommand(int controllerIndex) :Command(controllerIndex) {};
		~MoveCommand() override = default;
		void Execute(const int& parameter = 0) override;
		void Release(const int& parameter = 0) override;
		void Undo() override;
	};

}

