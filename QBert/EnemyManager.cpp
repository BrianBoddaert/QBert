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

using namespace dae;

EnemyManager::EnemyManager()
	: m_SpawnTimer{0.0f}
	, m_SpawnEnemyInterval{7.5f}
	, m_CoilyCount{0}
{

}
void EnemyManager::Update(float deltaT)
{
	m_SpawnTimer += deltaT;

	if (m_SpawnTimer >= m_SpawnEnemyInterval)
	{
		m_SpawnTimer = 0.0f;
		SpawnCoily();
	}
}


void EnemyManager::SpawnCoily()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	SDL_Surface* windowSurface = Minigin::GetWindowSurface();

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
	CollisionManager::GetInstance().AddCollider(coily);

	scene->Add(coily);
	m_CoilyCount++;
}

void EnemyManager::RemoveEnemyByName(const std::string& name)
{
	auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	auto& collisionManager = CollisionManager::GetInstance();
	currentScene->RemoveObjectsByName(name);
	collisionManager.RemoveObjectsByName(name);

	m_CoilyCount--;
}


void EnemyManager::ClearEnemies()
{
	auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	auto& collisionManager = CollisionManager::GetInstance();
	currentScene->RemoveObjectsByTag(dae::Tag::Coily);
	collisionManager.RemoveCollidersByTag(dae::Tag::Coily);

	m_CoilyCount = 0;
}