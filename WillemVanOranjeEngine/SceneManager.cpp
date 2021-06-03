#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaT)
{
	m_pCurrentScene->Update(deltaT);
}

void dae::SceneManager::Render()
{
	m_pCurrentScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name,const int gameMode)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name,(GameMode)gameMode));
	m_Scenes.push_back(scene);

	m_pCurrentScene = scene;
	return *m_pCurrentScene;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetSceneByName(const std::string& n) const
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetTag().compare(n) == 0)
			return scene;
	}
	return nullptr;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetCurrentScene()
{
	return m_pCurrentScene;
}

void dae::SceneManager::SetCurrentScene(const std::shared_ptr<Scene>& scene)
{
	m_pCurrentScene = scene;
}
