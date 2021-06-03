#pragma once
#include "Component.h"
#include <glm\vec2.hpp>
#include "Scene.h"
#include <SDL.h>
#include "ControlComponent.h"
#include "Cube.h"

namespace dae
{
	class MapComponent final : public Component
	{
	public:
		enum class Level
		{
			LevelOne,
			LevelTwo,
			LevelThree
		};
		MapComponent(Scene& scene, const glm::vec2& highestCubePos);
		const glm::vec2& GetCubeOffset() const;
		void Update(float) override;
		void Render(const glm::vec2&, const glm::vec2&) const override;
		std::shared_ptr<Cube> GetCube(int index) { return m_Cubes[index]; }
		bool GetNextCubeIndex(int& currentIndex, const QBertSprite& dir) const; // Returns false if the player jumps off the map
		bool LevelCompleteCheck() const;
		void SetLevelFinished(bool value) { m_LevelFinished = value; }
		const Level& GetCurrentLevel() const { return m_CurrentLevel; };
	private:
		void Initialize(Scene& scene);
		void CreateMap(Scene& scene);
		void CreateCube(const size_t& index, const glm::vec2& pos, Scene& scene);

		int GetColumnNumber(const int& currentTileIndex) const;



	private:
		void NextMap();
		const static int m_MaxCubes = 28;
		Level m_CurrentLevel;
		const int m_FirstRowCubeCount;
		const int m_CubeColumnCount;
		const SDL_Rect m_CubeSrcRect;
		glm::vec2 m_CubeOffset;
		const glm::vec2 m_HighestCubePos;

		const static int m_SideLength = 7;
		int m_MostLeftBlocks[m_SideLength];
		int m_MostRightBlocks[m_SideLength];
		int m_LowestBlocks[m_SideLength];
		std::shared_ptr<Cube> m_Cubes[m_MaxCubes];

		bool m_LevelFinished;
		float m_TileColorFlashTimer;
		float m_TileColorFlashDuration;
		int m_LevelFinishedColorChangeCount;
		int m_LevelFinishedColorChangeLimit;

	};


}
