#include "MiniginPCH.h"
#include "MapComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ControlComponent.h"
#include "MoveComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "TxtLoader.h"
//Static variables Init

using namespace dae;

const int m_MaxCubes = 28;
const int m_SideLength = 7;

MapComponent::MapComponent(Scene& scene, const dae::Vector3& highestCubePos)
	:m_FirstRowCubeCount(7)
	, m_CubeColumnCount(7)
	, m_CubeSrcRect{ 0,160,32,32 }
	, m_CubeOffset{ 16,24 }
	, m_HighestCubePos{ highestCubePos }
	, m_LevelFinished{ false }
	, m_TileColorFlashTimer{ 0.0f }
	, m_TileColorFlashDuration{ 0.3f }
	, m_LevelFinishedColorChangeLimit{ 8 }
	, m_LevelFinishedColorChangeCount{ 0 }
	, m_CurrentLevel{Level::LevelOne}
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

void MapComponent::LoadMap(dae::Scene& scene)
{

	TxtLoader txtLoader("../Data/Map.txt");
	std::vector<dae::Vector2>cubePositions = txtLoader.ReadVec3();

	for (size_t i = 0; i < cubePositions.size(); i++)
	{
		dae::Vector2 pos = m_HighestCubePos + cubePositions[i];
		CreateCube(i, dae::Vector3(pos.x, pos.y, 0), scene);
	}

}

void MapComponent::Initialize(Scene& scene)
{
	LoadMap(scene);
}

bool MapComponent::LevelCompleteCheck() const
{
	for (size_t i = 0; i < m_MaxCubes; i++)
	{
		if (!m_Cubes[i]->GetActivated())
			return false;
	}

	return true;
}
int MapComponent::GetRowNumber(const int& currentTileIndex) const
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

int MapComponent::GetZNumber(const int& currentTileIndex) const
{
	int currentZ = 0;
	int indexCounter = 0;
	for (int i = 0; i < m_FirstRowCubeCount; i++)
	{
		indexCounter = i;
		currentZ = i;
		for (int j = 0; j < i+1; j++)
		{
			if (indexCounter == currentTileIndex)
				return currentZ;

			indexCounter += 6 - j;
		}

	}

	return -1;

}

int MapComponent::GetColumnNumber(const int& currentTileIndex) const
{
	int cubeColumnCount = m_CubeColumnCount;
	int columnIndex = 0;
	int indexCounter = 0;

	for (int i = 0; i < m_FirstRowCubeCount; i++)
	{
		columnIndex = i;
		indexCounter = i;
		for (int j = 0; j < cubeColumnCount; j++)
		{

			if (currentTileIndex == indexCounter)
				return columnIndex;

			indexCounter += (m_CubeColumnCount - j);
		}
		cubeColumnCount--;
	}

	return -1;
}


void MapComponent::CreateCube(const size_t& index, const dae::Vector3& pos, Scene&)
{
	m_Cubes[index] = std::make_shared<Cube>((int)index);
	auto gameObj = m_Cubes[index]->GetGameObject();
	float scale = GAMESCALE;
	gameObj->AddComponent(new TransformComponent(pos, scale));
	gameObj->AddComponent(new RenderComponent(m_CubeSrcRect));
	gameObj->SetTexture("Textures/Qbert2.png");

}

bool MapComponent::SetNextCubeIndexAndCheckIfItsOnACube(int& currentIndex, const DirectionSprite& dir, bool sideWalking) const
// Returns false if the player jumps off the map
// Alters the currentIndex
{
	int rowIndex = GetRowNumber(currentIndex);
	int columnIndex = GetColumnNumber(currentIndex);
	int zIndex = GetZNumber(currentIndex);
	switch (dir)
	{
	case DirectionSprite::DownLeftJump:
	{
		if (sideWalking)
		{
			if (columnIndex == 0)
			{
				return false;
			}
			currentIndex += m_FirstRowCubeCount - zIndex + columnIndex - 1;
		}
		else
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_LowestBlocks[i])
					return false;

			currentIndex += m_FirstRowCubeCount - rowIndex;

		}

		break;
	}
	case DirectionSprite::UpRightJump:
	{
		if (sideWalking)
		{
			if (columnIndex == zIndex)
			{
				return false;
			}
			currentIndex -= m_FirstRowCubeCount - zIndex + columnIndex + 1;
			
		}
		else
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_MostRightBlocks[i])
					return false;

			int columnIndexAfterJump = rowIndex - 1;
			currentIndex -= m_FirstRowCubeCount - columnIndexAfterJump;

		}

		break;
	}
	case DirectionSprite::UpLeftJump:
	{
		if (sideWalking)
		{
			if (columnIndex == 0)
			{
				return false;
			}
			--currentIndex;
		}
		else
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_MostLeftBlocks[i])
					return false;

			currentIndex--;
		}


		break;
	}
	case DirectionSprite::DownRightJump:
	{
		if (sideWalking)
		{
			if (columnIndex == zIndex)
			{
				return false;
			}
			currentIndex -= m_FirstRowCubeCount - zIndex + columnIndex;
			break;
		}
		else
		{
			for (size_t i = 0; i < m_SideLength; i++)
				if (currentIndex == m_LowestBlocks[i])
					return false;

			currentIndex++;
		}


		break;
	}

	}

	return true;
}

void MapComponent::Update(float deltaT)
{
	if (m_LevelFinished)
	{
		m_TileColorFlashTimer += deltaT;

		if (m_TileColorFlashTimer > m_TileColorFlashDuration)
		{
			m_TileColorFlashTimer = 0;
			m_LevelFinishedColorChangeCount++;
			for (auto& cube : m_Cubes)
			{
				cube->SetColorNext();
			}

			if (m_LevelFinishedColorChangeCount == m_LevelFinishedColorChangeLimit)
			{
				NextMap();
			}
		}

	}

	for (auto& cube : m_Cubes)
	{
		cube->Update(deltaT);
	}


}

void MapComponent::NextMap()
{
	m_CurrentLevel = Level(int(m_CurrentLevel)+1);
	InputManager::GetInstance().LockInput(false);
	m_LevelFinished = false;
	m_LevelFinishedColorChangeCount = 0;
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	if (scene->GetGameMode() == GameMode::SinglePlayer || scene->GetGameMode() == GameMode::Versus)
	{
		auto player = scene->GetPlayer(0);
		TransformComponent* transformComp = player->GetComponent<TransformComponent>();
		MoveComponent* moveComp = player->GetComponent<MoveComponent>();
		ControlComponent* playerComp = player->GetComponent<ControlComponent>();
		moveComp->SetCurrentCubeIndex(moveComp->GetStartCubeIndex());
		if (transformComp)
		{
			const auto& playerSpawn = playerComp->GetSpawnPosition();
			transformComp->SetPosition(playerSpawn.x, playerSpawn.y, playerSpawn.z);
		}
	}
	else if (scene->GetGameMode() == GameMode::CoOp)
	{
		auto player1 = scene->GetPlayer(0);
		TransformComponent* transformComp = player1->GetComponent<TransformComponent>();
		MoveComponent* moveComp = player1->GetComponent<MoveComponent>();
		ControlComponent* playerComp = player1->GetComponent<ControlComponent>();
		moveComp->SetCurrentCubeIndex(moveComp->GetStartCubeIndex());
		if (transformComp)
		{
			const auto& playerSpawn = playerComp->GetSpawnPosition();
			transformComp->SetPosition(playerSpawn.x, playerSpawn.y, playerSpawn.z);
		}

		auto player2 = scene->GetPlayer(1);
		transformComp = player2->GetComponent<TransformComponent>();
		moveComp = player2->GetComponent<MoveComponent>();
		playerComp = player2->GetComponent<ControlComponent>();
		moveComp->SetCurrentCubeIndex(moveComp->GetStartCubeIndex());
		if (transformComp)
		{
			const auto& playerSpawn = playerComp->GetSpawnPosition();
			transformComp->SetPosition(playerSpawn.x, playerSpawn.y, playerSpawn.z);
		}
	}

	for (const auto& cube : m_Cubes)
	{
		if (cube)
			cube->SetActivated(false);
	}
}

void MapComponent::Render(const dae::Vector2&, const dae::Vector2&) const
{

	for (const auto& cube : m_Cubes)
	{
		if (cube)
			cube->Render();
	}
}

const dae::Vector2& MapComponent::GetCubeOffset() const
{
	return m_CubeOffset;
}
