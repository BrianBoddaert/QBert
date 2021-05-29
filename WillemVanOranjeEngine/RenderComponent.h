#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <SDL.h>

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(Texture2D* texture);
		RenderComponent();
		RenderComponent(const SDL_Rect& src);

		~RenderComponent()override;
		 void Update(float deltaT) override;
		 void Render(const glm::vec2& pos, const glm::vec2& scale) const override;
		 Texture2D* GetTexture();
		 void SetTexture(Texture2D* texture);
		 const SDL_Rect& GetSrcRect() const;
		 void SetSrcRect(const SDL_Rect&);

		 const glm::vec2& GetSpritePixelSize() { return m_SpritePixelSize; };
	private:

		const glm::vec2 m_SpritePixelSize;

		Texture2D* m_pTexture{ nullptr };

		SDL_Rect m_SrcRect;

	};

}