#pragma once
#include "Command.h"

namespace dae
{
	class IncreasePointsCommand final :
		public Command
	{
	public:
		IncreasePointsCommand(int index) :Command(index) {};
		~IncreasePointsCommand() override = default;
		void Execute(const int& parameter = 0) override;
		void Release(const int& parameter = 0) override;
		void Undo() override;
	};


}

