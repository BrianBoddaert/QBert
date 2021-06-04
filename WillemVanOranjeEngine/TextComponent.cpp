#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(const std::string& text, Font* font, float wrap)
	:m_NeedsUpdate(true)
	, m_Text(text)
	, m_pFont(font)
	, m_pTexture(nullptr)
	, m_Wrap{ wrap }
{ }

void dae::TextComponent::Update(float)
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		int width {};
		int height{};

		SDL_Surface* surf;
		if (m_Wrap)
		{
			TTF_SizeText(m_pFont->GetFont(), m_Text.c_str(), &width, &height);
			 surf = TTF_RenderText_Blended_Wrapped(m_pFont->GetFont(), m_Text.c_str(), color, Uint32(width * m_Wrap));
		}
		else
		{
			 surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		}

		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render(const dae::Vector2& pos, const dae::Vector2& scale) const
{
	if (m_pTexture != nullptr)
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y,scale);
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

//void dae::TextComponent::SetPosition(const float x, const float y)
//{
//	m_Transform->SetPosition(x, y, 0.0f);
//}


