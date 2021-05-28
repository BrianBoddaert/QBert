#pragma once
#include "Component.h"
#include <glm\vec2.hpp>
#include "Scene.h"
#include <SDL.h>
#include "PlayerComponent.h"
#include "Cube.h"

namespace dae
{
	class MapComponent final : public Component
	{
	public:
		MapComponent(Scene& scene, const glm::vec2& highestCubePos);
		const glm::vec2& GetCubeOffset() const;
		void Update(float) override;
		void Render(const glm::vec2&, const glm::vec2&) const override;
		std::shared_ptr<Cube> GetCube(int index) { return m_Cubes[index]; }
		bool GetNextCubeIndex(int& currentIndex, const QBertSprite& dir) const; // Returns false if the player jumps off the map

	private:
		void Initialize(Scene& scene);
		void CreateMap(Scene& scene);
		void CreateCube(const size_t& index, const glm::vec2& pos, Scene& scene);
		
		int GetColumnNumber(const int& currentTileIndex) const;
	private:

		const static int m_MaxCubes = 28;

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

	};


}
