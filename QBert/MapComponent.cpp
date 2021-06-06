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
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "TextComponent.h"
#include "ScoreComponent.h"
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


	srand(unsigned int(time(NULL)));
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
	SpawnDiscs();
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

	for (size_t i = 0; i < m_Discs.size(); i++)
	{
		m_Discs[i]->Update(deltaT);

		if (m_Discs[i]->GetMarkedForDelete())
		{
			dae::SceneManager::GetInstance().GetCurrentScene()->RemoveObjectsByObject(m_Discs[i]->GetGameObject());
			CollisionManager::GetInstance().RemoveColliderByObject(m_Discs[i]->GetGameObject());
			m_Discs.erase(m_Discs.begin() + i);
		}
	}

}

void MapComponent::MovePlayerToSpawn(const std::string& name)
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	auto player = scene->GetObjectByName(name);

	if (!player)
		return;

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

void MapComponent::NextMap()
{
	InputManager::GetInstance().LockInput(false);
	m_LevelFinished = false;
	m_LevelFinishedColorChangeCount = 0;
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	SpawnDiscs();

	if (scene->GetGameMode() == GameMode::SinglePlayer || scene->GetGameMode() == GameMode::Versus)
	{
		MovePlayerToSpawn("Player1");
	}
	else if (scene->GetGameMode() == GameMode::CoOp)
	{
		MovePlayerToSpawn("Player1");
		MovePlayerToSpawn("Player2");
	}

	for (const auto& cube : m_Cubes)
	{
		if (cube)
			cube->SetActivated(false);
	}
	{
		if (m_CurrentLevel != Level::LevelThree)
		m_CurrentLevel = Level(int(m_CurrentLevel) + 1);
		std::string text = "Level: ";
		text += std::to_string(int(m_CurrentLevel) + 1); // Level 0 is displayed as 1
		scene->GetObjectByName("LevelDisplay")->GetComponent<dae::TextComponent>()->SetText(text);
		EnemyManager::GetInstance().Reset();
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

void MapComponent::SpawnDiscs()
{
	for (size_t i = 0; i < m_MaxCubes; i++)
	{
		m_Cubes[i]->SetNextToDisc(false);
	}
	for (size_t i = 0; i < m_Discs.size(); i++)
	{
		auto players = SceneManager::GetInstance().GetCurrentScene()->GetPlayers();
		for (size_t j = 0; j < players.size(); j++)
		{
			players[j]->GetComponent<ScoreComponent>()->IncreaseScore(dae::Event::DiscLeftAtEndOfTheStage);
		}
		m_Discs[i]->SetMarkedForDelete(true);
	}

	float offsetFromTheSide = 10;

	int leftSideCubeIndex = m_MostLeftBlocks[rand() % 5 + 2];
	int rightSideCubeIndex = m_MostRightBlocks[rand() % 4 + 2];

	Vector3 disc1Pos = m_Cubes[leftSideCubeIndex]->GetGameObject()->GetComponent<TransformComponent>()->GetPosition();
	Vector3 disc2Pos = m_Cubes[rightSideCubeIndex]->GetGameObject()->GetComponent<TransformComponent>()->GetPosition();

	disc1Pos.y -= offsetFromTheSide;
	disc2Pos.y -= offsetFromTheSide;
	disc1Pos.x -= offsetFromTheSide;
	disc2Pos.x += m_Cubes[rightSideCubeIndex]->GetGameObject()->GetComponent<RenderComponent>()->GetSrcRect().w/2 + offsetFromTheSide;

	m_Cubes[leftSideCubeIndex]->SetNextToDisc(true);
	m_Cubes[rightSideCubeIndex]->SetNextToDisc(true);

	CreateDisc(disc1Pos, *dae::SceneManager::GetInstance().GetCurrentScene());
	CreateDisc(disc2Pos, *dae::SceneManager::GetInstance().GetCurrentScene());

}

std::shared_ptr<Disc> MapComponent::GetDiscByGameObject(const std::shared_ptr<dae::GameObject>& go) const
{
	for (size_t i = 0; i < m_Discs.size(); i++)
	{
		if (m_Discs[i]->GetGameObject() == go)
		{
			return m_Discs[i];
		}
	}

	return nullptr;
}
void MapComponent::CreateDisc(const Vector3& pos, Scene& scene)
{
	Vector2 discSrcRect{16,16};
	m_Discs.push_back(std::make_shared<Disc>(m_Cubes[0]->GetGameObject()->GetComponent<TransformComponent>()->GetPosition(), discSrcRect));
	m_Discs[m_Discs.size() - 1]->GetGameObject()->AddTag(dae::Tag::Disc);
	auto newDisc = m_Discs[m_Discs.size() - 1]->GetGameObject();

	newDisc->AddComponent(new TransformComponent(pos,1));
	//0,352 16x16
	SDL_Rect srcRect = { 0,352,16,16 };

	newDisc->AddComponent(new RenderComponent(srcRect));
	newDisc->SetTexture("Textures/Qbert2.png");
	
	CollisionManager::GetInstance().AddCollider(newDisc);
	scene.Add(newDisc);
}

const dae::Vector2& MapComponent::GetCubeOffset() const
{
	return m_CubeOffset;
}


bool MapComponent::DoesCubeHaveDiscNextToIt(int cubeIndex) const
{
	return m_Cubes[cubeIndex]->GetNextToDisc();
}