#pragma once
#include <memory>
#include <SDL.h>
#include <stdio.h>

//struct SDL_Window;



class Minigin
{
public:
	void Initialize();
	void AssignKeys();
	void LoadGame() const;
	void LoadSinglePlayerScene() const;
	void LoadCoOpScene() const;
	void LoadVersusScene() const;
	void Cleanup();
	void Run();

	static SDL_Surface* GetWindowSurface() { return m_WindowSurface; }
private:
	static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps		
	static float MsPerUpdate;
	SDL_Window* m_Window{};
	static SDL_Surface* m_WindowSurface;
};
