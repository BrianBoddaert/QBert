#pragma once
#include "BaseCollisionManager.h"
#include "Singleton.h"

class CollisionManager final : public dae::Singleton<CollisionManager>,  public dae::BaseCollisionManager
{
public:

private:
	friend class dae::Singleton<CollisionManager>;

    void CollisionEffect(std::shared_ptr<dae::GameObject> player, std::shared_ptr<dae::GameObject> collider) override;
};

