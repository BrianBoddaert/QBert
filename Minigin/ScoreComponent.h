#pragma once
#include "Component.h"

namespace dae
{

	class TextComponent;
	class ScoreComponent : public Component
	{
	public:
		ScoreComponent(const unsigned int& score);
		void Update(float deltaT) override;
		const unsigned int& GetScore() const;
		void SetScore(const unsigned int&);
		void IncreaseScore(Event e);
	private:
		unsigned int m_Score;

	};
}