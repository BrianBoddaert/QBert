#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include "GameObject.h"
#pragma warning(pop)

namespace dae
{
	class Font;
	class Texture2D;
	class Component
	{
	public:

		virtual void Update(float) {};
		virtual void Render(const glm::vec2&, const glm::vec2&) const {};
		virtual ~Component() {};

		void SetGameObject(GameObject* obj) { m_pGameObject = obj; }
		GameObject* GetGameObject() { return m_pGameObject; }

	private:
	protected:
		GameObject* m_pGameObject;
	};

}