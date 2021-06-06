#pragma once
#include "GameObject.h"

class Cube
{
public:
	enum class CubeColor
	{
		Red,
		Yellow,
		Blue
	};

	Cube(int id);
	~Cube() = default;

	Cube(const Cube& other) = delete;
	Cube(Cube&& other) = delete;
	Cube& operator=(const Cube& other) = delete;
	Cube& operator=(Cube&& other) = delete;

	void Update(float);
	void Render() const;

	bool SetActivated(bool value);

	const int GetId() const { return m_Id; };
	std::shared_ptr<dae::GameObject> GetGameObject() const { return m_pGameObject; };

	void SetColorNext(bool = true);
	void SetColor(const CubeColor& color);

	bool GetActivated() const { return m_Activated; };

	bool GetNextToDisc() const { return m_NextToDisc; }
	void SetNextToDisc(bool v) { m_NextToDisc = v; }

private:

	CubeColor m_CurrentColor;
	std::shared_ptr<dae::GameObject> m_pGameObject;

	bool m_Activated;
	bool m_IntermediateActivated;
	bool m_NextToDisc;
	const int m_Id;
};

