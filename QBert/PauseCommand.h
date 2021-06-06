#pragma once
#include "Command.h"

namespace dae
{
	class PauseCommand final :
		public Command
	{
	public:
		PauseCommand(int index) :Command(index) {};
		~PauseCommand() override = default;
		PauseCommand(const PauseCommand&) = delete;
		PauseCommand& operator=(const PauseCommand&) = delete;
		PauseCommand(PauseCommand&&) = delete;
		PauseCommand& operator= (PauseCommand&&) = delete;

		void Execute(const int& parameter = 0) override;
		void Release(const int& parameter = 0) override;
		void Undo() override;
	};


}

