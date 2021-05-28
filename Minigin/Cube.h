#pragma once
#include "GameObject.h"

class Cube
{
public:
	Cube(int id);
	void Color();

	~Cube() = default;

	Cube(const Cube & other) = delete;
	Cube(Cube && other) = delete;
	Cube& operator=(const Cube & other) = delete;
	Cube& operator=(Cube && other) = delete;

	void Update(float);
	void Render() const;


	const int GetId() const{ return m_Id; };
	std::shared_ptr<dae::GameObject> GetGameObject() const { return m_pGameObject; };
private:

	std::shared_ptr<dae::GameObject> m_pGameObject;

	bool m_Colored;
	const int m_Id;
};

