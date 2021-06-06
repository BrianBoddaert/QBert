#pragma once
#include <memory>
#include <SDL.h>
#include <stdio.h>
#include "Structs.h"
#include "Singleton.h"
#include "Scene.h"
//struct SDL_Window;



class Minigin final : public dae::Singleton<Minigin>
{
public:
	void Initialize();
	void AssignKeys();
	void LoadGame() const;
	void LoadSinglePlayerScene() const;
	void LoadCoOpScene() const;
	void LoadVersusScene() const;
	void LoadHUD(dae::Scene& scene) const;
	void Cleanup();
	void Run();
	void SetPaused(bool v) { m_Paused = v; }
	bool GetPaused() const { return m_Paused;}
	void SetGameOver(bool v) { m_GameOver = v; }
	bool GetGameOver() const { return m_GameOver; }
	void LoadSceneByGameMode(dae::GameMode gameMode) const;
	void ClearGame();
	static SDL_Surface* GetWindowSurface() { return m_WindowSurface; }
private:
	friend class dae::Singleton<Minigin>;
	static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps		
	static float MsPerUpdate;
	bool m_Paused, m_GameOver;
	SDL_Window* m_Window{};
	static SDL_Surface* m_WindowSurface;
};
