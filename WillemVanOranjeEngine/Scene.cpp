#include <iostream>
#include <algorithm>
#include "Scene.h"
#include "SceneManager.h"
#include "TransformComponent.h"

using namespace dae;

Scene::Scene(const std::string& name,const GameMode& gameMode)
	: m_Name(name)
	, m_GameMode{gameMode}
{
}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float deltaT)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Update(deltaT);
	}

}

bool CompareZAxis(const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b)
{
	auto aTransform = a->GetComponent<TransformComponent>();
	auto bTransform = b->GetComponent<TransformComponent>();
	float a_z;
	float b_z;
	if (aTransform)
		a_z = aTransform->GetPosition().z;
	else
		a_z = 0.0;

	if (bTransform)
		b_z = bTransform->GetPosition().z;
	else
		b_z = 0.0;

	return a_z < b_z;
}

void Scene::SortOnZAxis()
{
	std::sort(m_Objects.begin(), m_Objects.end(), CompareZAxis);
}


void Scene::Render() const
{

	//for (const auto& player : m_pPlayers)
	//{
	//		player->Render();
	//}
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

const std::string& Scene::GetTag() const
{
	return m_Name;
}

std::shared_ptr<GameObject> Scene::GetObjectByName(const std::string& name) const
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
	m_Objects.push_back(player);
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
void Scene::RemoveObjectsByTag(const dae::Tag& tag)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]->HasTag(tag) && m_Objects[i]->GetName() != "Player2")
			m_Objects.erase(m_Objects.begin() + i);
	}
}

void Scene::RemoveObjectsByName(const std::string& name)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]->GetName() == name)
			m_Objects.erase(m_Objects.begin() + i);
	}
}

void Scene::AddMap(const std::shared_ptr<GameObject>& map)
{
	m_pMaps.push_back(map);
	m_Objects.push_back(map);
	m_CurrentMap = map;
}

std::shared_ptr<GameObject> Scene::Getmap(int index) const
{
	return m_pMaps[index];
}