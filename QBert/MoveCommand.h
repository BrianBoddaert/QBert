#pragma once
#include "Command.h"

enum class MoveInputDirections
	{
		Up,
		Right,
		Down,
		Left,
		Count
	};

class MoveCommand : public dae::Command
{
public:
	MoveCommand(int controllerIndex) : dae::Command(controllerIndex) {};
	~MoveCommand() override = default;
	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};


