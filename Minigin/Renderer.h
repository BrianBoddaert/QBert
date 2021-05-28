#pragma once
#include "Singleton.h"
#include <SDL.h>
#include "glm/vec2.hpp"

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, const glm::vec2& scale = { 1,1 }) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const glm::vec2& scale = { 1,1 }) const;
		void RenderTexture(const Texture2D& texture, const float x, const float y, const SDL_Rect& src, const glm::vec2& scale = { 1,1 }) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect& dst, const SDL_Rect& src, const glm::vec2& scale = { 1,1 }) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		SDL_Renderer* m_Renderer{};
	};
}

