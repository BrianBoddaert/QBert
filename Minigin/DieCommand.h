#pragma once
#include "Command.h"

namespace dae
{
	class DieCommand final :
		public Command
	{
	public:
		DieCommand(int index) :Command(index) {};
		~DieCommand() override = default;
		void Execute(const int& parameter = 0) override;
		void Release(const int& parameter = 0) override;
		void Undo() override;
	};


}

