#pragma once
#include "Component.h"
#include "Structs.h"
#include "Scene.h"
#include <SDL.h>
#include "ControlComponent.h"
#include "Cube.h"
#include "Disc.h"

class MapComponent final : public dae::Component
{
public:
	enum class Level
	{
		LevelOne,
		LevelTwo,
		LevelThree
	};
	MapComponent(dae::Scene& scene, const dae::Vector3& highestCubePos);
	const dae::Vector2& GetCubeOffset() const;
	void Update(float) override;
	void Render(const dae::Vector2&, const dae::Vector2&) const override;
	std::shared_ptr<Cube> GetCube(int index) { return m_Cubes[index]; }
	bool SetNextCubeIndexAndCheckIfItsOnACube(int& currentIndex, const dae::DirectionSprite& dir, bool sideWalking = false) const;
	// Returns false if the player jumps off the map ^
	bool LevelCompleteCheck() const;
	void SetLevelFinished(bool value) { m_LevelFinished = value; }
	const Level& GetCurrentLevel() const { return m_CurrentLevel; };
	void MovePlayerToSpawn(const std::string& name);
	bool DoesCubeHaveDiscNextToIt(int cubeIndex) const;
	std::shared_ptr<Disc> GetDiscByGameObject(const std::shared_ptr<dae::GameObject>& go) const;
	const dae::Vector3& GetHighestCubePos() const { return m_HighestCubePos; }
private:
	void Initialize(dae::Scene& scene);
	void LoadMap(dae::Scene& scene);
	void CreateCube(const size_t& index, const dae::Vector3& pos, dae::Scene& scene);

	int GetRowNumber(const int& currentTileIndex) const;
	int GetColumnNumber(const int& currentTileIndex) const;
	int GetZNumber(const int& currentTileIndex) const;
	void CreateDisc(const dae::Vector3& pos, dae::Scene& scene);
	void SpawnDiscs();


private:
	void NextMap();
	const static int m_MaxCubes = 28;
	Level m_CurrentLevel;
	const int m_FirstRowCubeCount;
	const int m_CubeColumnCount;
	const SDL_Rect m_CubeSrcRect;
	dae::Vector2 m_CubeOffset;
	const dae::Vector3 m_HighestCubePos;

	const static int m_SideLength = 7;
	int m_MostLeftBlocks[m_SideLength];
	int m_MostRightBlocks[m_SideLength];
	int m_LowestBlocks[m_SideLength];
	std::shared_ptr<Cube> m_Cubes[m_MaxCubes];
	std::vector<std::shared_ptr<Disc>> m_Discs;
	bool m_LevelFinished;
	float m_TileColorFlashTimer;
	float m_TileColorFlashDuration;
	int m_LevelFinishedColorChangeCount;
	int m_LevelFinishedColorChangeLimit;

};



