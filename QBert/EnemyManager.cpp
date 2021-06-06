#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "MoveComponent.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "Minigin.h"
#include "PathFindingComponent.h"
#include "SidewaysMoveComponent.h"
using namespace dae;

EnemyManager::EnemyManager()
	: m_SpawnTimer{0.0f}
	, m_SpawnEnemyInterval{3.5f}
	, m_CoilyCount{0}
	, m_WrongWayCount{0}
	, m_SlickSamCount{0}
{
	srand(unsigned int(time(NULL)));
}

void EnemyManager::Update(float deltaT)
{
	m_SpawnTimer += deltaT;

	if (m_SpawnTimer >= m_SpawnEnemyInterval)
	{
		m_SpawnTimer = 0.0f;

		bool whatToSpawn = std::rand() % 2;

		if (SceneManager::GetInstance().GetCurrentScene()->GetObjectsByTag(dae::Tag::Coily).size() == 0)
			SpawnCoily();
		else if (whatToSpawn)
			SpawnSlickSam();
		else
			SpawnWrongWay();
	}
}

void EnemyManager::SpawnWrongWay()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	SDL_Surface* windowSurface = Minigin::GetWindowSurface();

	bool WrongWayOrUgg = std::rand() % 2;

	std::string name = "WrongWay";

	name += std::to_string(m_WrongWayCount);
	auto wrongWay = std::make_shared<GameObject>(name);

	int y;
	if (WrongWayOrUgg)
		y = 96;
	else
		y = 112;

	SDL_Rect srcRect = { 0,y,16,16 };
	const dae::Vector2 srcRectHalf = { 8,8 };
	wrongWay->AddComponent(new RenderComponent(srcRect));
	wrongWay->SetTexture("Textures/Qbert2.png");

	dae::Vector3 pos;
	int tileIndex;
	if (WrongWayOrUgg)
	{
		pos = dae::Vector3{ (windowSurface->w / 2 + srcRectHalf.x) - 96, (windowSurface->h / 2 - srcRectHalf.y) + 144,6 };
		tileIndex = 27;
	}
	else
	{
		pos = dae::Vector3{ (windowSurface->w / 2 + srcRectHalf.x) + 96, (windowSurface->h / 2 - srcRectHalf.y) + 144,6 };
		tileIndex = 6;
	}


	wrongWay->AddComponent(new TransformComponent(pos, 1.0f));

	wrongWay->AddComponent(new HealthComponent(1));

	wrongWay->AddComponent(new SidewaysMoveComponent(tileIndex, WrongWayOrUgg));
	wrongWay->AddTag(dae::Tag::WrongWay);
	wrongWay->AddTag(dae::Tag::Enemy);
	CollisionManager::GetInstance().AddCollider(wrongWay);

	scene->Add(wrongWay);
	m_WrongWayCount++;
}

void EnemyManager::SpawnCoily()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	SDL_Surface* windowSurface = Minigin::GetWindowSurface();

	if (scene->GetGameMode() == dae::GameMode::Versus)
	{
	auto coily = std::make_shared<GameObject>("Player2");

	SDL_Rect srcRect = { 0,32,16,32 };
	const dae::Vector2 srcRectHalf = { 8,16 };
	coily->AddComponent(new RenderComponent(srcRect));
	coily->SetTexture("Textures/Qbert2.png");

	const Vector3 pos = { (windowSurface->w / 2 + srcRectHalf.x), (windowSurface->h / 2 - srcRectHalf.y),0 };

	coily->AddComponent(new TransformComponent(pos, 1.0f));

	coily->AddComponent(new HealthComponent(1));
	coily->AddComponent(new MoveComponent(0));
	coily->AddComponent(new ControlComponent(pos));
	coily->AddTag(dae::Tag::Coily);
	CollisionManager::GetInstance().AddCollider(coily);

	scene->AddPlayer(coily);
	}
	else
	{
		std::string name = "Coily";

		name += std::to_string(m_CoilyCount);
		auto coily = std::make_shared<GameObject>(name);

		SDL_Rect srcRect = { 0,32,16,32 };
		const dae::Vector2 srcRectHalf = { 8,16 };
		coily->AddComponent(new RenderComponent(srcRect));
		coily->SetTexture("Textures/Qbert2.png");

		const Vector3 pos = { (windowSurface->w / 2 + srcRectHalf.x), (windowSurface->h / 2 - srcRectHalf.y),0 };

		coily->AddComponent(new TransformComponent(pos, 1.0f));

		coily->AddComponent(new HealthComponent(1));
		coily->AddComponent(new MoveComponent(0));
		coily->AddComponent(new PathFindingComponent());
		coily->AddTag(dae::Tag::Coily);
		coily->AddTag(dae::Tag::Enemy);
		CollisionManager::GetInstance().AddCollider(coily);

		scene->Add(coily);

	}
	m_CoilyCount++;


}

void EnemyManager::SpawnSlickSam()
{
	bool SlickOrSam = std::rand() % 2;


	auto scene = SceneManager::GetInstance().GetCurrentScene();
	SDL_Surface* windowSurface = Minigin::GetWindowSurface();

	std::string name = "SlickSam";

	name += std::to_string(m_SlickSamCount);
	auto slicksam = std::make_shared<GameObject>(name);

	int srcRectY;
	if (SlickOrSam)
		srcRectY = 128;
	else
		srcRectY = 144;

	const SDL_Rect srcRect = { 0,srcRectY,16,16 };
	const dae::Vector2 srcRectHalf = { 8,8 };
	slicksam->AddComponent(new RenderComponent(srcRect));
	slicksam->SetTexture("Textures/Qbert2.png");

	const Vector3 pos = { (windowSurface->w / 2 + srcRectHalf.x), (windowSurface->h / 2 - srcRectHalf.y),0 };

	slicksam->AddComponent(new TransformComponent(pos, 1.0f));

	slicksam->AddComponent(new HealthComponent(1));
	slicksam->AddComponent(new MoveComponent(0));
	slicksam->AddComponent(new PathFindingComponent());
	slicksam->AddTag(dae::Tag::SlickSam);
	slicksam->AddTag(dae::Tag::Enemy);
	CollisionManager::GetInstance().AddCollider(slicksam);

	scene->Add(slicksam);
	m_SlickSamCount++;
}

void EnemyManager::RemoveEnemyByName(const std::string& name)
{
	auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	auto& collisionManager = CollisionManager::GetInstance();
	currentScene->RemoveObjectsByName(name);
	collisionManager.RemoveCollidersByName(name);
}


void EnemyManager::ClearEnemies()
{
	auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	auto& collisionManager = CollisionManager::GetInstance();
	currentScene->RemoveObjectsByTag(dae::Tag::Enemy);
	collisionManager.RemoveCollidersByTag(dae::Tag::Enemy);
}

void EnemyManager::ResetTimer()
{
	m_SpawnTimer = 0.0f;
}


void EnemyManager::Reset()
{
	ResetTimer();
	ClearEnemies();
}