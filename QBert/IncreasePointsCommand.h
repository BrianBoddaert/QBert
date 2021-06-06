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
		IncreasePointsCommand(const IncreasePointsCommand&) = delete;
		IncreasePointsCommand& operator=(const IncreasePointsCommand&) = delete;
		IncreasePointsCommand(IncreasePointsCommand&&) = delete;
		IncreasePointsCommand& operator= (IncreasePointsCommand&&) = delete;

		void Execute(const int& parameter = 0) override;
		void Release(const int& parameter = 0) override;
		void Undo() override;
	};


}

