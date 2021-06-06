#pragma once
#include "Singleton.h"

class EnemyManager : public dae::Singleton<EnemyManager>
{
public:
	EnemyManager();
	void Update(float deltaT);
	void SpawnCoily();
	void SpawnWrongWay();
	void SpawnSlickSam();
	void ClearEnemies();
	void RemoveEnemyByName(const std::string& Enemy);
	void ResetTimer();
	void Reset();
	
private:
	friend class dae::Singleton<EnemyManager>;

	float m_SpawnTimer;
	float m_SpawnEnemyInterval;
	int m_CoilyCount;
	int m_WrongWayCount;
	int m_SlickSamCount;
};

