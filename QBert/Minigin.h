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
	bool SetPlayerDied();
	bool SetPaused(bool v);
	bool GetGameOver() const { return m_Gamestate == dae::GameState::GameOver; }
	bool GetPaused() const { return m_Gamestate == dae::GameState::Paused; }
	void SetGameState(const dae::GameState& state) { m_Gamestate = state; }
	const dae::GameState& GetGameState() const { return m_Gamestate; }
	void LoadSceneByGameMode(dae::GameMode gameMode) const;
	void ClearGame();
	static SDL_Surface* GetWindowSurface() { return m_WindowSurface; }
	void SetSkipPlayerDiedDisplayTimer() { m_SkipPlayerDiedDisplayTimer = true; }
	bool GetSkipPlayerDiedDisplayTimer() const {  return m_SkipPlayerDiedDisplayTimer; }
private:
	friend class dae::Singleton<Minigin>;

	void PlayerDiedTimer(float deltaT);
	float m_PlayerDiedDisplayTimer = 0.0f;
	const float m_PlayerDiedDisplayDuration = 1.5f;
	bool m_SkipPlayerDiedDisplayTimer = false;

	static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps		
	static float MsPerUpdate;
	dae::GameState m_Gamestate;
	SDL_Window* m_Window{};
	static SDL_Surface* m_WindowSurface;
};
