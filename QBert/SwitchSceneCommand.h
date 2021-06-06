#pragma once
#include <Command.h>
class SwitchSceneCommand :
    public dae::Command
{
public:
	SwitchSceneCommand(int controllerIndex) : dae::Command(controllerIndex) {};
	~SwitchSceneCommand() override = default;
	SwitchSceneCommand(const SwitchSceneCommand&) = delete;
	SwitchSceneCommand& operator=(const SwitchSceneCommand&) = delete;
	SwitchSceneCommand(SwitchSceneCommand&&) = delete;
	SwitchSceneCommand& operator= (SwitchSceneCommand&&) = delete;
	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};

