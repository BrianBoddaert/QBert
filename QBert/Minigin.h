#pragma once
#include <memory>
#include <SDL.h>
#include <stdio.h>
//struct SDL_Window;

namespace dae
{

	class Minigin
	{
	public:
		void Initialize();
		void AssignKeys();
		void LoadGame() const;
		void Cleanup();
		void Run();

	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps		
		static float MsPerUpdate;
		SDL_Window* m_Window{};
	};
}