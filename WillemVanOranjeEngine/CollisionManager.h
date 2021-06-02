#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include <memory>

namespace dae
{
	class CollisionManager final : public dae::Singleton<CollisionManager >
	{
	public:
		CollisionManager();
		void Update();
		void AddCollider(const std::shared_ptr<GameObject>& gameObject);
	private:
		friend class dae::Singleton<CollisionManager>;

		bool IsColliding(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2);
		void CollisionEffect(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> collider);

		std::vector<std::shared_ptr<GameObject>> m_pColliders;
		std::vector<std::shared_ptr<GameObject>> m_pPlayers;
	};

}
