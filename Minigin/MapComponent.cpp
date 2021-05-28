#include "MiniginPCH.h"
#include "MapComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PlayerComponent.h"

//Static variables Init
const int m_MaxCubes = 28;
const int m_SideLength = 7;

dae::MapComponent::MapComponent(dae::Scene& scene, const glm::vec2& highestCubePos)
	:m_FirstRowCubeCount(7)
	, m_CubeColumnCount(7)
	, m_CubeSrcRect{ 0,160,32,32 }
	, m_CubeOffset{ 16,24 }
	, m_HighestCubePos{ highestCubePos }
{

	int mostLeftBlockIndex = 0;
	int lowestBlockIndex = 6;
	for (int i = 0; i < m_SideLength; i++)
	{
		m_MostLeftBlocks[i] = mostLeftBlockIndex;
		mostLeftBlockIndex += m_SideLength - i;

		m_MostRightBlocks[i] = i;

		m_LowestBlocks[i] = lowestBlockIndex; // +1 because its the bottom
		lowestBlockIndex += 6 - i;
	}

	
	m_CubeOffset.x *= GAMESCALE;
	m_CubeOffset.y *= GAMESCALE;
	Initialize(scene);
}

void dae::MapComponent::Initialize(dae::Scene& scene)
{
	CreateMap(scene);
}


void dae::MapComponent::CreateMap(dae::Scene& scene)
{
	//16 x24
	int indexCounter = 0;
	int rowCubeCount = m_FirstRowCubeCount;
	glm::vec2 highestCubePos = m_HighestCubePos;

	for (size_t j = 0; j < m_CubeColumnCount; j++)
	{
		for (size_t i = 0; i < rowCubeCount; i++)
		{

			glm::vec2 pos = highestCubePos;
			pos.x += m_CubeOffset.x * i;
			pos.y += m_CubeOffset.y * i;

			CreateCube(indexCounter, pos, scene);
			indexCounter++;
		}
		 highestCubePos.x -= m_CubeOffset.x;
		 highestCubePos.y += m_CubeOffset.y;

		 rowCubeCount--;
	}
}
int dae::MapComponent::GetColumnNumber(const int& currentTileIndex) const
{
	int cubeCount = m_FirstRowCubeCount;

	for (int i = 0; i < m_CubeColumnCount; i++)
	{
		if (currentTileIndex < cubeCount)
			return i;

		cubeCount += (m_FirstRowCubeCount - (i + 1)); // + i because it's not an index but a count
	}

	return -1;

}

void dae::MapComponent::CreateCube(const size_t& index, const glm::vec2& pos, dae::Scene&)
{
	m_Cubes[index] = std::make_shared<Cube>((int)index);
	auto gameObj = m_Cubes[index]->GetGameObject();
	float scale = GAMESCALE;
	gameObj->AddComponent(new dae::TransformComponent(pos, scale));
	gameObj->AddComponent(new dae::RenderComponent(m_CubeSrcRect));
	gameObj->SetTexture("Textures/Qbert.png");
	
}



bool dae::MapComponent::GetNextCubeIndex(int& currentIndex, const dae::QBertSprite& dir) const 
// Returns false if the player jumps off the map
{
	int columnIndex = GetColumnNumber(currentIndex);

	switch (dir)
	{
		case QBertSprite::DownLeftJump:
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_LowestBlocks[i])
					return false;

			currentIndex += m_FirstRowCubeCount - columnIndex;

			break;
		}
		case QBertSprite::UpRightJump:
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_MostRightBlocks[i])
					return false;

			int columnIndexAfterJump = columnIndex - 1;
			currentIndex -= m_FirstRowCubeCount - columnIndexAfterJump;

			break;
		}
		case QBertSprite::UpLeftJump:
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_MostLeftBlocks[i])
					return false;

			currentIndex--;

			break;
		}
		case QBertSprite::DownRightJump:
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_LowestBlocks[i])
					return false;

			currentIndex++;

			break;
		}

	}

	return true;
}

void dae::MapComponent::Update(float deltaT)
{
	for (auto& cube : m_Cubes)
	{
		if (cube)
			cube->Update(deltaT);
	}
}

void dae::MapComponent::Render(const glm::vec2&, const glm::vec2&) const
{
	for (const auto& cube : m_Cubes)
	{
		if (cube)
			cube->Render();
	}
}

const glm::vec2& dae::MapComponent::GetCubeOffset() const
{
	return m_CubeOffset;
}

