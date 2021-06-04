#pragma once
#include "Singleton.h"

class EnemyManager : public dae::Singleton<EnemyManager>
{
public:
	EnemyManager();
	void Update(float deltaT);
	void SpawnCoily();
private:
	friend class dae::Singleton<EnemyManager>;


	float m_SpawnTimer;
	float m_SpawnEnemyInterval;
};

