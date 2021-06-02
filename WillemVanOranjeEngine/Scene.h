#pragma once
#include "SceneManager.h"
#include <memory>
#include "GameObject.h"

namespace dae
{
	enum class GameMode
	{
		SinglePlayer,
		CoOp,
		Versus
	};

	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name,const int gameMode);
	public:
		void Add(const std::shared_ptr<SceneObject>& object);

		void Update(float deltaT);
		void Render() const;
		const std::string& GetName() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<SceneObject> GetObjectByName(const std::string& name) const; 
		void AddPlayer(const std::shared_ptr<GameObject>& player);
		std::shared_ptr<GameObject> GetPlayer(int index) const;
		void AddMap(const std::shared_ptr<GameObject>& map);
		std::shared_ptr<GameObject> Getmap(int index) const;
		void SetCurrentMap(const std::shared_ptr<GameObject>&);
		std::shared_ptr<GameObject> GetCurrentMap() const;

		const GameMode& GetGameMode() const { return m_GameMode; };
	private: 
		explicit Scene(const std::string& name, const GameMode& gameMode);

		const GameMode m_GameMode;
		const std::string m_Name;
		std::vector< std::shared_ptr<GameObject>> m_pPlayers{};

		std::vector < std::shared_ptr<SceneObject>> m_Objects{};

		std::vector< std::shared_ptr<GameObject>> m_pMaps{};
		std::shared_ptr<GameObject> m_CurrentMap;
	};

}
