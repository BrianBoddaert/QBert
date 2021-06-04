#pragma once
#include "Singleton.h"

class EnemyManager : public dae::Singleton<EnemyManager>
{
public:
	EnemyManager();
	void Update(float deltaT);
	void SpawnCoily();
	void ClearEnemies();
	void RemoveEnemyByName(const std::string& Enemy);
private:
	friend class dae::Singleton<EnemyManager>;

	float m_SpawnTimer;
	float m_SpawnEnemyInterval;
	int m_CoilyCount;
};

