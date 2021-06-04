#pragma once
#include <Command.h>
class SwitchSceneCommand :
    public dae::Command
{
public:
	SwitchSceneCommand(int controllerIndex) : dae::Command(controllerIndex) {};
	~SwitchSceneCommand() override = default;
	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};

