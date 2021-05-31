#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

namespace dae
{

	class TransformComponent : public Component
	{
	public:
		TransformComponent(const glm::vec2& pos, const glm::vec2& scale = { 1,1 });
		TransformComponent(const glm::vec2& pos, float scale = 1.0f);
		TransformComponent();
		const glm::vec2& GetPosition() const;
		void SetPosition(const glm::vec2& pos);
		void SetPosition(float x, float y);

		const glm::vec2& GetScale() const;
		void SetScale(float x, float y);
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Scale;

	};
}
