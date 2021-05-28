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
		void Execute(const int& parameter = 0) override;
		void Release(const int& parameter = 0) override;
		void Undo() override;
	};


}

