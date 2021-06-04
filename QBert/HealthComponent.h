#pragma once
#include "Component.h"



	class TextComponent;
	class HealthComponent : public dae::Component
	{
	public:
		HealthComponent(const unsigned int& health);
		void Update(float deltaT) override;
		const unsigned int& GetLives() const;
		void SetLives(const unsigned int&);
		void Die();
		void Respawn();
		bool IsAlive() const;
	private:
		unsigned int m_Lives;
		bool m_Dead;
	};
