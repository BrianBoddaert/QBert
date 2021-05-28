#include "MiniginPCH.h"
#include <iostream>
#include "PauseCommand.h"

// TEMP
#include "AudioClasses.h"

void dae::PauseCommand::Execute(const int&)
{
	std::cout << "Pause" << std::endl;
	ServiceLocator::GetSoundSystem().TogglePause();
}

void dae::PauseCommand::Release(const int&)
{

}

void dae::PauseCommand::Undo()
{

}