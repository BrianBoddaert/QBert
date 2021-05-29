#include <iostream>
#include "TransformComponent.h"

using namespace dae;

TransformComponent::TransformComponent(const glm::vec2& pos,const glm::vec2& scale)
	:m_Position{pos}
	,m_Scale{scale}
{
}
TransformComponent::TransformComponent(const glm::vec2& pos,float scale)
	: m_Position{ pos }
	, m_Scale{ scale }
{
}
TransformComponent::TransformComponent()
	:m_Position{ 0,0 }
{

}

void TransformComponent::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}


const glm::vec2& TransformComponent::GetPosition() const 
{
	return m_Position; 
}

void TransformComponent::SetScale(const float x, const float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}


const glm::vec2& TransformComponent::GetScale() const
{
	return m_Scale;
}