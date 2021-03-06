#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <thread>
//#include <SDL.h>
#include <SDL_mixer.h>

#include "GameObject.h"
#include "Scene.h"

#include "AudioClasses.h"

#include "ControlComponent.h"
#include "TransformComponent.h"
#include "FPSComponent.h"
#include "HealthComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "ScoreComponent.h"
#include "TimerComponent.h"
#include "MapComponent.h"
#include "MoveComponent.h"

#include "IncreasePointsCommand.h"
#include "PauseCommand.h"
#include "SwitchSceneCommand.h"

#include "LivesObserver.h"
#include "ScoreObserver.h"
#include "LevelCompleteObserver.h"
#include "CollisionManager.h"
#include "EnemyManager.h"

#include <Xinput.h>
#include <glm\vec2.hpp>


using namespace std;
using namespace std::chrono;
using namespace dae;

float Minigin::MsPerUpdate = 0.02f;
SDL_Surface* Minigin::m_WindowSurface = nullptr;

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());


	if (SDL_Init(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());

	int frequency = 44100;
	int chunkSize = 2048;
	int channels = 2;

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
		throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());

	//Mix_Music* bgm = Mix_LoadMUS("music.mp3"); // Wav, mp3 both work
	//Mix_Chunk* soundEffect = Mix_LoadWAV("soundEffect.wav"); // Wav files

	m_Window = SDL_CreateWindow(
		"QBert",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	m_WindowSurface = SDL_GetWindowSurface(m_Window);

	ServiceLocator::SetSoundSystem(new LoggingSoundSystem(new SdlSoundSystem()));
	ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Jump, "../Data/Audio/jump.mp3");
	ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Fall, "../Data/Audio/fall.mp3");
	ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Lift, "../Data/Audio/lift.mp3");
	ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Victory, "../Data/Audio/victory.mp3");
	//ServiceLocator::GetSoundSystem().AddSoundToLibrary(MusicId::Ambient, "../Data/royaltyFreeSong.mp3");

	//ServiceLocator::GetSoundSystem().QueueSound(MusicId::Ambient,0.3f);
	////Mix_Music* music = Mix_LoadMUS("../Data/mortalkombat.mp3"); // Wav, mp3 both work

	//ServiceLocator::GetSoundSystem().QueueSound(AudioId::Fire, 1.0f);

}

/**
 * Code constructing the scene world starts here
 */

void Minigin::AssignKeys()
{
	auto& input = InputManager::GetInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		//input.AssignControllerKey<DieCommand>(ControllerButton::LeftThumbStickUp, i);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickUp, i, (int)MoveInputDirections::Up);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickLeft, i, (int)MoveInputDirections::Left);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickRight, i, (int)MoveInputDirections::Right);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickDown, i, (int)MoveInputDirections::Down);

		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonUp, i, (int)MoveInputDirections::Up);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonLeft, i, (int)MoveInputDirections::Left);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonRight, i, (int)MoveInputDirections::Right);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonDown, i, (int)MoveInputDirections::Down);

		input.AssignControllerKey<PauseCommand>(ControllerButton::ButtonStart, i);
		input.AssignControllerKey<SwitchSceneCommand>(ControllerButton::TriggerRight, i);
	}

	input.AssignKeyboardKey<PauseCommand>(KeyboardButton::P);

	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::W, (int)MoveInputDirections::Up);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::A, (int)MoveInputDirections::Left);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::S, (int)MoveInputDirections::Down);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::D, (int)MoveInputDirections::Right);

	input.AssignKeyboardKey<SwitchSceneCommand>(KeyboardButton::PLUS, 0);
}

void Minigin::LoadSinglePlayerScene() const
{


		auto& scene = SceneManager::GetInstance().CreateScene("SinglePlayerScene", (int)GameMode::SinglePlayer);
	LoadHUD(scene);

	{
		auto map = std::make_shared<GameObject>("Map");
		//250,200
		const dae::Vector3 highestCubePos = { float(m_WindowSurface->w / 2), float(m_WindowSurface->h / 2),0 };
		map->AddComponent(new MapComponent(scene, highestCubePos));
		map->AddWatcher(new LevelCompleteObserver());
		scene.AddMap(map);
	}
	{
		auto player = std::make_shared<GameObject>("Player1");
		const dae::Vector2 playerHalfSize = { 8,8 };
		const dae::Vector3 playerPos = { m_WindowSurface->w / 2 + playerHalfSize.x, m_WindowSurface->h / 2 - playerHalfSize.y,1 };
		ControlComponent* controlComponent = new ControlComponent(playerPos);  // ControlComponent();

		player->AddComponent(controlComponent);

		SDL_Rect playerSrcRect = { 0,0,16,16 };

		player->AddComponent(new RenderComponent(playerSrcRect));
		player->SetTexture("Textures/Qbert2.png");

		player->AddComponent(new TransformComponent(playerPos, 1.0f));
		//player->SetPosition(playerPos.x, playerPos.y);

		player->AddComponent(new HealthComponent(3));
		player->AddComponent(new ScoreComponent(0));

		player->AddWatcher(new LivesObserver());
		player->AddWatcher(new ScoreObserver());
		player->AddComponent(new MoveComponent(0));
		player->AddTag(dae::Tag::Player);
		player->AddTag(dae::Tag::Player1);
		CollisionManager::GetInstance().AddCollider(player);
		scene.AddPlayer(player);
	}

	scene.SortOnZAxis();
}

void Minigin::LoadCoOpScene() const
{

	auto& scene = SceneManager::GetInstance().CreateScene("CoOpScene", (int)GameMode::CoOp);

	LoadHUD(scene);
	auto map = std::make_shared<GameObject>("Map");
	//250,200
	const dae::Vector3 highestCubePos = { (float)m_WindowSurface->w / 2,  (float)m_WindowSurface->h / 2,0 };
	MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	map->AddComponent(mapComp);
	map->AddWatcher(new LevelCompleteObserver());
	scene.AddMap(map);
	
	{

		{
			auto player = std::make_shared<GameObject>("Player1");

			const dae::Vector2 playerHalfSize = { 8,8 };
			const dae::Vector3 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) - 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,7 };

			ControlComponent* controlComponent = new ControlComponent(playerPos);

			player->AddComponent(controlComponent);
			SDL_Rect playerSrcRect = { 0,0,16,16 };

			player->AddComponent(new RenderComponent(playerSrcRect));
			player->SetTexture("Textures/Qbert2.png");
			//const dae::Vector2& cubeOffset =  mapComp->GetCubeOffset();

			player->AddComponent(new TransformComponent(playerPos, 1.0f));

			player->AddComponent(new HealthComponent(3));
			player->AddComponent(new ScoreComponent(0));

			player->AddWatcher(new LivesObserver());
			player->AddWatcher(new ScoreObserver());
			player->AddComponent(new MoveComponent(27));
			player->AddTag(dae::Tag::Player);
			player->AddTag(dae::Tag::Player1);
			CollisionManager::GetInstance().AddCollider(player);
			scene.AddPlayer(player);
		}
		{
			auto player = std::make_shared<GameObject>("Player2");

			SDL_Rect playerSrcRect = { 0,0,16,16 };
			const dae::Vector2 playerHalfSize = { 8,8 };
			player->AddComponent(new RenderComponent(playerSrcRect));
			player->SetTexture("Textures/Qbert2.png");
			//const dae::Vector2& cubeOffset =  mapComp->GetCubeOffset();
			const dae::Vector3 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) + 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,7 };
			ControlComponent* controlComponent = new ControlComponent(playerPos);

			player->AddComponent(controlComponent);

			player->AddComponent(new TransformComponent(playerPos, 1.0f));

			player->AddComponent(new HealthComponent(3));
			player->AddComponent(new ScoreComponent(0));

			player->AddWatcher(new LivesObserver());
			player->AddWatcher(new ScoreObserver());
			player->AddComponent(new MoveComponent(6));
			player->AddTag(dae::Tag::Player);
			player->AddTag(dae::Tag::Player2);
			CollisionManager::GetInstance().AddCollider(player);
			scene.AddPlayer(player);
		}
	}
	scene.SortOnZAxis();
}

void Minigin::LoadVersusScene() const
{

	auto& scene = SceneManager::GetInstance().CreateScene("VersusScene", (int)GameMode::Versus);
	LoadHUD(scene);
	auto map = std::make_shared<GameObject>("Map");
	//250,200
	const dae::Vector3 highestCubePos = { (float)m_WindowSurface->w / 2,  (float)m_WindowSurface->h / 2,0 };
	MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	map->AddComponent(mapComp);
	map->AddWatcher(new LevelCompleteObserver());
	scene.AddMap(map);

	{

		{
			auto player = std::make_shared<GameObject>("Player1");

			const dae::Vector2 playerHalfSize = { 8,8 };
			const dae::Vector3 playerPos = { m_WindowSurface->w / 2 + playerHalfSize.x, m_WindowSurface->h / 2 - playerHalfSize.y,0 };

			ControlComponent* controlComponent = new ControlComponent(playerPos);

			player->AddComponent(controlComponent);
			SDL_Rect playerSrcRect = { 0,0,16,16 };

			player->AddComponent(new RenderComponent(playerSrcRect));
			player->SetTexture("Textures/Qbert2.png");
			//const dae::Vector2& cubeOffset =  mapComp->GetCubeOffset();

			player->AddComponent(new TransformComponent(playerPos, 1.0f));

			player->AddComponent(new HealthComponent(3));
			player->AddComponent(new ScoreComponent(0));

			player->AddWatcher(new LivesObserver());
			player->AddWatcher(new ScoreObserver());
			player->AddComponent(new MoveComponent(0));
			player->AddTag(dae::Tag::Player);
			player->AddTag(dae::Tag::Player1);
			CollisionManager::GetInstance().AddCollider(player);
			scene.AddPlayer(player);
		}
	}
	scene.SortOnZAxis();
}

void Minigin::LoadHUD(dae::Scene& scene) const
{
	{
		
		auto player1HUD = std::make_shared<GameObject>("HUDPlayer1");
		player1HUD->AddComponent(new TransformComponent({ 20,50.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		player1HUD->AddComponent(new dae::TextComponent("Player1", font));

		scene.Add(player1HUD);
	}
	{
		auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer1");
		scoreDisplay->AddComponent(new TransformComponent({ 20,70.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto scoreCounter = new dae::TextComponent("Score: 0", font);
		scoreDisplay->AddComponent(scoreCounter);

		scene.Add(scoreDisplay);
	}
	{
		auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer1");
		livesDisplay->AddComponent(new TransformComponent({ 20.0f ,90.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
		livesDisplay->AddComponent(livesCounter);

		scene.Add(livesDisplay);
	}
	if (scene.GetGameMode() == dae::GameMode::CoOp)
	{
		{

			auto player2HUD = std::make_shared<GameObject>("HUDPlayer2");
			player2HUD->AddComponent(new TransformComponent({ 20,120.0f,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			player2HUD->AddComponent(new dae::TextComponent("Player2", font));

			scene.Add(player2HUD);
		}
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer2");
			scoreDisplay->AddComponent(new TransformComponent({ 20,140.0f,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new dae::TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);
		}
		{
			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer2");
			livesDisplay->AddComponent(new TransformComponent({ 20.0f ,160.0f,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}
	}
	{
		auto levelDisplay = std::make_shared<GameObject>("LevelDisplay");
		levelDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w -100.0f ,100.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto levelCounter = new dae::TextComponent("Level: 1", font);
		levelDisplay->AddComponent(levelCounter);

		scene.Add(levelDisplay);
	}
	{
		auto roundDisplay = std::make_shared<GameObject>("RoundDisplay");
		roundDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w - 100.0f ,130.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto roundCounter = new dae::TextComponent("Round: 1", font);
		roundDisplay->AddComponent(roundCounter);

		scene.Add(roundDisplay);
	}
	{
		auto pausedDisplay = std::make_shared<GameObject>("PauseDisplay");
		pausedDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w/2 - 30.0f ,m_WindowSurface->h/2 - 10.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
		auto pausedText = new dae::TextComponent("Paused", font);

		pausedDisplay->AddComponent(pausedText);
		pausedDisplay->SetEnabled(false);
		scene.Add(pausedDisplay);
	}
	{
		auto gameOverDisplay = std::make_shared<GameObject>("GameOverDisplay");
		gameOverDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w / 2 - 40.0f ,m_WindowSurface->h / 2 - 10.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
		auto gameOverText = new dae::TextComponent("Game Over", font);

		gameOverDisplay->AddComponent(gameOverText);
		gameOverDisplay->SetEnabled(false);
		scene.Add(gameOverDisplay);
	}
	{
		auto gameOverDisplay = std::make_shared<GameObject>("PressToRestart");
		gameOverDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w / 2 - 100.0f ,m_WindowSurface->h / 2 + 20.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto gameOverText = new dae::TextComponent("Press start or P to restart", font);

		gameOverDisplay->AddComponent(gameOverText);
		gameOverDisplay->SetEnabled(false);
		scene.Add(gameOverDisplay);
	}
	{
		auto go = std::make_shared<GameObject>("Swearing");
		go->AddComponent(new TransformComponent({ m_WindowSurface->w / 2.0f ,m_WindowSurface->h / 2.0f ,10 }, 1.0f));

		SDL_Rect srcRect = { 128,83,48,25 };
		go->AddComponent(new RenderComponent(srcRect));
		go->SetTexture("Textures/Qbert2.png");
		go->SetEnabled(false);
		scene.Add(go);
	}
}

void Minigin::LoadSceneByGameMode(dae::GameMode gamemode) const
{
	if (gamemode == dae::GameMode::SinglePlayer)
		LoadSinglePlayerScene();
	else if (gamemode == dae::GameMode::CoOp)
		LoadCoOpScene();
	else if (gamemode == dae::GameMode::Versus)
		LoadVersusScene();
}

void Minigin::LoadGame() const
{

	//LoadCoOpScene();
	//LoadVersusScene();
	LoadSinglePlayerScene();

}

void Minigin::Cleanup()
{
	ServiceLocator::CleanUp();

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);


	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();

}

void Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	AssignKeys();
	LoadGame();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& enemyManager = EnemyManager::GetInstance();
	auto& collisionManager = CollisionManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;
	auto lastTime = high_resolution_clock::now();
	float lag = 0.0f;
	m_Gamestate = GameState::Playing;
	std::thread audioThread(&SoundSystem::Update, &ServiceLocator::GetSoundSystem());

	while (doContinue)
	{

		const auto currentTime = high_resolution_clock::now();
		float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		doContinue = input.ProcessInput();

		while (lag >= MsPerUpdate)
		{
			if (m_Gamestate == GameState::Playing)
			{
				sceneManager.Update(deltaTime);
				enemyManager.Update(deltaTime);
				collisionManager.Update(deltaTime);
			}
			else if (m_Gamestate == GameState::PlayerDied)
			{
				PlayerDiedTimer(deltaTime);
			}
			lag -= MsPerUpdate;
		}

		//audioThread.join();

		renderer.Render();

		auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
		this_thread::sleep_for(sleepTime);
	}

	audioThread.detach();
	Cleanup();
}

void Minigin::PlayerDiedTimer(float deltaT)
{
	m_PlayerDiedDisplayTimer += deltaT;

	if (m_PlayerDiedDisplayTimer >= m_PlayerDiedDisplayDuration || m_SkipPlayerDiedDisplayTimer)
	{
		m_SkipPlayerDiedDisplayTimer = false;
		m_PlayerDiedDisplayTimer = 0.0f;
		m_Gamestate = GameState::Playing;

		EnemyManager::GetInstance().Reset();

		auto scene = SceneManager::GetInstance().GetCurrentScene();
		scene->GetCurrentMap()->GetComponent<MapComponent>()->MovePlayerToSpawn("Player1");
		scene->GetCurrentMap()->GetComponent<MapComponent>()->MovePlayerToSpawn("Player2");


		auto players = scene->GetPlayers();
		for (size_t i = 0; i < players.size(); i++)
		{
			auto moveComp = players[i]->GetComponent<MoveComponent>();
			moveComp->SetIsMoving(false);
		}
		auto swearing = SceneManager::GetInstance().GetCurrentScene()->GetObjectByName("Swearing");
		swearing->SetEnabled(false);
		// Turn swear bubble off

	}
}
void Minigin::ClearGame()
{
	EnemyManager::GetInstance().Reset();
	CollisionManager::GetInstance().ClearColliders();

	auto& sceneManager = dae::SceneManager::GetInstance();
	auto scene = sceneManager.GetCurrentScene();
	scene->ClearObjects();
	m_Gamestate = GameState::Playing;
	dae::GameMode gamemode = scene->GetGameMode();
	sceneManager.RemoveCurrentScene();

	LoadSceneByGameMode(gamemode);
}

bool Minigin::SetPaused(bool v)
{
	if (v)
	{
		if (m_Gamestate == dae::GameState::Playing)
		{
			m_Gamestate = dae::GameState::Paused;
			return true;
		}

	}
	else
	{
		if (m_Gamestate == dae::GameState::Paused)
		{
			m_Gamestate = dae::GameState::Playing;
			return true;
		}

	}

	return false;

}

bool Minigin::SetPlayerDied()
{
	if (m_Gamestate == dae::GameState::Playing)
	{
		m_Gamestate = dae::GameState::PlayerDied;
		return true;
	}

	return false;

}