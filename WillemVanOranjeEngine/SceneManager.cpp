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
	m_pScenes.push_back(scene);

	m_pCurrentScene = scene;
	return *m_pCurrentScene;
}

void dae::SceneManager::RemoveCurrentScene()
{
	for (size_t i = 0; i < m_pScenes.size(); i++)
	{
		if (m_pScenes[i] == m_pCurrentScene)
		m_pScenes.erase(m_pScenes.begin() + i);
	}

}

std::shared_ptr<dae::Scene> dae::SceneManager::GetSceneByName(const std::string& n) const
{
	for (const auto& scene : m_pScenes)
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

//void dae::SceneManager::SetCurrentSceneToNext()
//{
//	int currentIndex = 0;
//	for (int i = 0; i < m_pScenes.size(); i++)
//	{
//		if (m_pScenes[i] == m_pCurrentScene)
//		{
//			currentIndex = i;
//			break;
//		}
//	}
//
//	if (currentIndex >= m_pScenes.size() - 1) // minus 1 because it's size to index
//		currentIndex = 0;
//	else
//		currentIndex++;
//
//	{
//		m_pCurrentScene = m_pScenes[currentIndex];
//	}
//
//
//}