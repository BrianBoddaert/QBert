#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include <memory>

namespace dae
{
	class BaseCollisionManager
	{
	public:
		BaseCollisionManager();
		void Update(float deltaT);
		void AddCollider(const std::shared_ptr<GameObject>& gameObject);
	protected:
	//	friend class dae::Singleton<BaseCollisionManager>;

		bool IsColliding(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2);
		virtual void CollisionEffect(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> collider) = 0;

		std::vector<std::shared_ptr<GameObject>> m_pColliders;
		std::vector<std::shared_ptr<GameObject>> m_pPlayers;
	};

}
