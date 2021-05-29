#include "MiniginPCH.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerComponent.h"
#include "MoveComponent.h"
using namespace dae;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float deltaT)
{
	m_CurrentMap->Update(deltaT);

	for(auto& object : m_Objects)
	{
		object->Update(deltaT);
	}
	for (const auto& player : m_pPlayers)
	{
		player->Update(deltaT);
	}
}

void Scene::Render() const
{
	for (const auto& player : m_pPlayers)
	{
		if (player->GetComponent<MoveComponent>()->GetIsFallingToDeathBehindMap())
			player->Render();
	}
	m_CurrentMap->Render();

	for (const auto& player : m_pPlayers)
	{
		if (!player->GetComponent<MoveComponent>()->GetIsFallingToDeathBehindMap())
			player->Render();
	}
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

std::shared_ptr<SceneObject> Scene::GetObjectByName(const std::string& name) const
{
	for (const auto& object : m_Objects)
	{
		if (object->GetName().compare(name) == 0)
		return object;
	}
	return nullptr;
}

void Scene::AddPlayer(const std::shared_ptr<GameObject>& player)
{
	m_pPlayers.push_back(player);
}

std::shared_ptr<GameObject> Scene::GetPlayer(int index) const
{
	if (m_pPlayers.size() <= (unsigned)index)
		return nullptr;

	return m_pPlayers[index];
}

void Scene::SetCurrentMap(const std::shared_ptr<GameObject>& map)
{
	m_CurrentMap = map;
}

std::shared_ptr<GameObject> Scene::GetCurrentMap() const
{
	return m_CurrentMap;
}

void Scene::AddMap(const std::shared_ptr<GameObject>& map)
{
	m_pMaps.push_back(map);
	m_CurrentMap = map;
}

std::shared_ptr<GameObject> Scene::Getmap(int index) const
{
	return m_pMaps[index];
}