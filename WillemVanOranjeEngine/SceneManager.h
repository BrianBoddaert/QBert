#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, const int gameMode);

		void Update(float deltaT);
		void Render();
		std::shared_ptr<Scene> GetSceneByName(const std::string& n) const;
		std::shared_ptr<Scene> GetCurrentScene();
		void SetCurrentScene(const std::shared_ptr<Scene>&);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_pCurrentScene;
	};
}
