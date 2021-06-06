#pragma once
#include "Component.h"


class TextComponent;
class ScoreComponent : public dae::Component
{
public:
	ScoreComponent(const unsigned int& score);
	void Update(float deltaT) override;
	const unsigned int& GetScore() const;
	void SetScore(const unsigned int&);
	void IncreaseScore(dae::Event e);
private:
	unsigned int m_Score;

};
