#include "RenderComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
using namespace dae;

RenderComponent::RenderComponent(Texture2D* texture)
	:m_pTexture{texture}
	, m_SrcRect{0,0,0,0}
	, m_SpritePixelSize{ 0,0 }
		
{
}

RenderComponent::RenderComponent()
	:m_SrcRect{ 0,0,0,0 }
	,m_SpritePixelSize{0,0}
{
}

RenderComponent::RenderComponent(const SDL_Rect& src)
	:m_SrcRect{ src }
	,m_SpritePixelSize{src.w,src.h}
{
}

RenderComponent::~RenderComponent()
{
	delete m_pTexture;
}

void RenderComponent::Update(float)
{


}

void RenderComponent::Render(const glm::vec2& pos, const glm::vec2& scale) const
{
	if (m_SrcRect.x == 0 && m_SrcRect.y == 0 && m_SrcRect.w == 0 && m_SrcRect.h == 0)
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, scale);
	}
	else
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, m_SrcRect, scale);
	}

}



Texture2D* RenderComponent::GetTexture()
{
	return m_pTexture;
}

void RenderComponent::SetTexture(Texture2D* texture)
{
	m_pTexture = texture;
}

const SDL_Rect& RenderComponent::GetSrcRect() const
{
	return m_SrcRect;
}
void  RenderComponent::SetSrcRect(const SDL_Rect& src)
{
	m_SrcRect = src;
}