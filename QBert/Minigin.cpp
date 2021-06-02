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

#include "Component.h"
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

#include "LivesObserver.h"
#include "ScoreObserver.h"
#include "LevelCompleteObserver.h"

#include <Xinput.h>
#include <glm\vec2.hpp>
#include "PlayerComponent.h"

using namespace std;
using namespace std::chrono;

float dae::Minigin::MsPerUpdate = 0.02f;

void dae::Minigin::Initialize()
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

	ServiceLocator::SetSoundSystem(new LoggingSoundSystem(new SdlSoundSystem()));
	ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Fire, "../Data/bruh.wav");
	//ServiceLocator::GetSoundSystem().AddSoundToLibrary(MusicId::Ambient, "../Data/royaltyFreeSong.mp3");

	//ServiceLocator::GetSoundSystem().QueueSound(MusicId::Ambient,0.3f);
//	Mix_Music* music = Mix_LoadMUS("../Data/mortalkombat.mp3"); // Wav, mp3 both work

	//ServiceLocator::GetSoundSystem().QueueSound(AudioId::Fire, 1.0f);

}

/**
 * Code constructing the scene world starts here
 */

void dae::Minigin::AssignKeys()
{
	auto& input = InputManager::GetInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		input.AssignControllerKey<PauseCommand>(ControllerButton::TriggerLeft, i);
		//input.AssignControllerKey<DieCommand>(ControllerButton::LeftThumbStickUp, i);
		input.AssignControllerKey<IncreasePointsCommand>(ControllerButton::RightThumbStickUp, i);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickUp, i, (int)MoveInputDirections::Up);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickLeft, i, (int)MoveInputDirections::Left);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickRight, i, (int)MoveInputDirections::Right);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickDown, i, (int)MoveInputDirections::Down);

		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonUp, i, (int)MoveInputDirections::Up);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonLeft, i, (int)MoveInputDirections::Left);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonRight, i, (int)MoveInputDirections::Right);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonDown, i, (int)MoveInputDirections::Down);
	}

	input.AssignKeyboardKey<PauseCommand>(KeyboardButton::P);

	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::W, (int)MoveInputDirections::Up);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::A, (int)MoveInputDirections::Left);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::S, (int)MoveInputDirections::Down);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::D, (int)MoveInputDirections::Right);
}

void dae::Minigin::LoadSinglePlayerScene() const
{
	SDL_Surface* windowSurface = SDL_GetWindowSurface(m_Window);

	auto& scene = SceneManager::GetInstance().CreateScene("SinglePlayerScene", (int)dae::GameMode::SinglePlayer);

	{
		auto go = std::make_shared<GameObject>("Background");
		//go->AddComponent(new RenderComponent());

		go->AddComponent(new TransformComponent());
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto textComp = new TextComponent("FPS", font);
		go->AddComponent(textComp);
		go->AddComponent(new FPSComponent(textComp));
		scene.Add(go);
	}
	{
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto go = std::make_shared<GameObject>("HowToPlay");
		go->AddComponent(new TransformComponent());
		go->AddComponent(new TextComponent("How to play: LeftThumbUp to die, RightThumbUp to earn 25 points, controller1 does this for player1, controller2 for player2, A,B,X and Y to fire, duck, fart and jump ", font, .4f));
		go->SetPosition(50, 150);
		scene.Add(go);
	}
	//go = std::make_shared<GameObject>("Logo");
	//go->AddComponent(new TransformComponent());
	//go->AddComponent(new RenderComponent());
	//go->SetTexture("logo.png");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//go = std::make_shared<GameObject>("Header");
	//go->AddComponent(new TransformComponent());
	//go->AddComponent(new TextComponent("Programming 4 Assignment", font));
	//go->SetPosition(80, 20);
	//scene.Add(go);
	{
		auto map = std::make_shared<GameObject>("Map");
		//250,200
		const glm::vec2 highestCubePos = { windowSurface->w / 2, windowSurface->h / 2 };
		map->AddComponent(new MapComponent(scene, highestCubePos));
		map->AddWatcher(new LevelCompleteObserver());
		scene.SetCurrentMap(map);
	}
	{
		auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer1");
		scoreDisplay->AddComponent(new TransformComponent({ 200,0 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto scoreCounter = new TextComponent("Score: 0", font);
		scoreDisplay->AddComponent(scoreCounter);

		scene.Add(scoreDisplay);

		auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer1");
		livesDisplay->AddComponent(new TransformComponent());
		livesDisplay->SetPosition(400, 0);

		font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto livesCounter = new TextComponent("Remaining lives: 3", font);
		livesDisplay->AddComponent(livesCounter);

		scene.Add(livesDisplay);

		auto player = std::make_shared<GameObject>("Player1");
		PlayerComponent* playerComponent = new PlayerComponent();

		player->AddComponent(playerComponent);

		SDL_Rect playerSrcRect = { 0,0,16,16 };
		const glm::vec2 playerHalfSize = { 8,8 };
		player->AddComponent(new RenderComponent(playerSrcRect));
		player->SetTexture("Textures/Qbert2.png");

		const glm::vec2 playerPos = { windowSurface->w / 2 + playerHalfSize.x, windowSurface->h / 2 - playerHalfSize.y };
		playerComponent->SetPlayerSpawn(playerPos);

		player->AddComponent(new TransformComponent(playerPos, 1.0f));
		//player->SetPosition(playerPos.x, playerPos.y);

		player->AddComponent(new HealthComponent(3));
		player->AddComponent(new ScoreComponent(0));

		player->AddWatcher(new LivesObserver());
		player->AddWatcher(new ScoreObserver());
		player->AddComponent(new MoveComponent(0));

		scene.AddPlayer(player);
	}
	{
		auto playerDiedPopup = std::make_shared<GameObject>("PlayerDiedPopup");
		playerDiedPopup->AddComponent(new TransformComponent());
		playerDiedPopup->AddComponent(new TimerComponent());
		playerDiedPopup->SetEnabled(false);
		playerDiedPopup->SetPosition(400, 400);
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto textComp = new TextComponent("Player died", font);
		playerDiedPopup->AddComponent(textComp);

		scene.Add(playerDiedPopup);
	}

}

void dae::Minigin::LoadCoOpScene() const
{
	SDL_Surface* windowSurface = SDL_GetWindowSurface(m_Window);

	auto& scene = SceneManager::GetInstance().CreateScene("CoOpScene", (int)dae::GameMode::CoOp);

	
	auto map = std::make_shared<GameObject>("Map");
	//250,200
	const glm::vec2 highestCubePos = { windowSurface->w / 2, windowSurface->h / 2 };
	MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	map->AddComponent(mapComp);
	map->AddWatcher(new LevelCompleteObserver());
	scene.SetCurrentMap(map);
	
	{
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer1");
			scoreDisplay->AddComponent(new TransformComponent({ 200,0 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);

			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer1");
			livesDisplay->AddComponent(new TransformComponent());
			livesDisplay->SetPosition(400, 0);

			font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer2");
			scoreDisplay->AddComponent(new TransformComponent({ windowSurface->w - 200.0f,0 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);

			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer2");
			livesDisplay->AddComponent(new TransformComponent());
			livesDisplay->SetPosition(windowSurface->w - 400.0f, 0);

			font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}

		{
			auto player = std::make_shared<GameObject>("Player1");
			PlayerComponent* playerComponent = new PlayerComponent();

			player->AddComponent(playerComponent);

			SDL_Rect playerSrcRect = { 0,0,16,16 };
			const glm::vec2 playerHalfSize = { 8,8 };
			player->AddComponent(new RenderComponent(playerSrcRect));
			player->SetTexture("Textures/Qbert2.png");
			//const glm::vec2& cubeOffset =  mapComp->GetCubeOffset();
			const glm::vec2 playerPos = { (windowSurface->w / 2 + playerHalfSize.x) - 96, (windowSurface->h / 2 - playerHalfSize.y) + 144 };
			playerComponent->SetPlayerSpawn(playerPos);

			player->AddComponent(new TransformComponent(playerPos, 1.0f));

			player->AddComponent(new HealthComponent(3));
			player->AddComponent(new ScoreComponent(0));

			player->AddWatcher(new LivesObserver());
			player->AddWatcher(new ScoreObserver());
			player->AddComponent(new MoveComponent(27));

			scene.AddPlayer(player);
		}
		{
			auto player = std::make_shared<GameObject>("Player2");
			PlayerComponent* playerComponent = new PlayerComponent();

			player->AddComponent(playerComponent);

			SDL_Rect playerSrcRect = { 0,0,16,16 };
			const glm::vec2 playerHalfSize = { 8,8 };
			player->AddComponent(new RenderComponent(playerSrcRect));
			player->SetTexture("Textures/Qbert2.png");
			//const glm::vec2& cubeOffset =  mapComp->GetCubeOffset();
			const glm::vec2 playerPos = { (windowSurface->w / 2 + playerHalfSize.x) + 96, (windowSurface->h / 2 - playerHalfSize.y) + 144 };
			playerComponent->SetPlayerSpawn(playerPos);

			player->AddComponent(new TransformComponent(playerPos, 1.0f));

			player->AddComponent(new HealthComponent(3));
			player->AddComponent(new ScoreComponent(0));

			player->AddWatcher(new LivesObserver());
			player->AddWatcher(new ScoreObserver());
			player->AddComponent(new MoveComponent(6));

			scene.AddPlayer(player);
		}
	}
	{
		auto playerDiedPopup = std::make_shared<GameObject>("PlayerDiedPopup");
		playerDiedPopup->AddComponent(new TransformComponent());
		playerDiedPopup->AddComponent(new TimerComponent());
		playerDiedPopup->SetEnabled(false);
		playerDiedPopup->SetPosition(400, 400);
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto textComp = new TextComponent("Player died", font);
		playerDiedPopup->AddComponent(textComp);

		scene.Add(playerDiedPopup);
	}
}

void dae::Minigin::LoadVersusScene() const
{

}


void dae::Minigin::LoadGame() const
{
	
	LoadCoOpScene();
	
	

}

void dae::Minigin::Cleanup()
{
	ServiceLocator::CleanUp();

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);


	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();

}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	AssignKeys();
	LoadGame();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;
	auto lastTime = high_resolution_clock::now();
	float lag = 0.0f;

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
			sceneManager.Update(deltaTime);
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

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// PLAYER EN MAP CLASS MAKEN MET EEN Gameobject variabel.
// Ze apart Updaten
// 
// glm::vector2 naar vector2Ints maken
// 
// 
// UIS FIXEN
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!