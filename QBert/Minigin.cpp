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

void Minigin::LoadSinglePlayerScene() const
{


	auto& scene = SceneManager::GetInstance().CreateScene("SinglePlayerScene", (int)GameMode::SinglePlayer);

	{
		auto go = std::make_shared<GameObject>("Background");
		//go->AddComponent(new RenderComponent());

		go->AddComponent(new TransformComponent());
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto textComp = new dae::TextComponent("FPS", font);
		go->AddComponent(textComp);
		go->AddComponent(new FPSComponent(textComp));
		scene.Add(go);
	}
	{
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
		auto go = std::make_shared<GameObject>("HowToPlay");
		go->AddComponent(new TransformComponent());
		go->AddComponent(new dae::TextComponent("How to play: LeftThumbUp to die, RightThumbUp to earn 25 points, controller1 does this for player1, controller2 for player2, A,B,X and Y to fire, duck, fart and jump ", font, .4f));
		go->SetPosition(50, 150,10);
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
	//go->AddComponent(new dae::TextComponent("Programming 4 Assignment", font));
	//go->SetPosition(80, 20);
	//scene.Add(go);
	{
		auto map = std::make_shared<GameObject>("Map");
		//250,200
		const dae::Vector3 highestCubePos = { float(m_WindowSurface->w / 2), float(m_WindowSurface->h / 2),0 };
		map->AddComponent(new MapComponent(scene, highestCubePos));
		map->AddWatcher(new LevelCompleteObserver());
		scene.AddMap(map);
	}
	{
		auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer1");
		scoreDisplay->AddComponent(new TransformComponent({ 200,0,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto scoreCounter = new dae::TextComponent("Score: 0", font);
		scoreDisplay->AddComponent(scoreCounter);

		scene.Add(scoreDisplay);

		auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer1");
		livesDisplay->AddComponent(new TransformComponent());
		livesDisplay->SetPosition(400, 0,10);

		font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
		livesDisplay->AddComponent(livesCounter);

		scene.Add(livesDisplay);

		auto player = std::make_shared<GameObject>("Player1");
		const dae::Vector2 playerHalfSize = { 8,8 };
		const dae::Vector3 playerPos = { m_WindowSurface->w / 2 + playerHalfSize.x, m_WindowSurface->h / 2 - playerHalfSize.y,0 };
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

		scene.AddPlayer(player);
	}
	{
		auto playerDiedPopup = std::make_shared<GameObject>("PlayerDiedPopup");
		playerDiedPopup->AddComponent(new TransformComponent());
		playerDiedPopup->AddComponent(new TimerComponent());
		playerDiedPopup->SetEnabled(false);
		playerDiedPopup->SetPosition(400, 400,10);
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto textComp = new dae::TextComponent("Player died", font);
		playerDiedPopup->AddComponent(textComp);

		scene.Add(playerDiedPopup);
	}

}

void Minigin::LoadCoOpScene() const
{

	auto& scene = SceneManager::GetInstance().CreateScene("CoOpScene", (int)GameMode::CoOp);

	
	auto map = std::make_shared<GameObject>("Map");
	//250,200
	const dae::Vector3 highestCubePos = { (float)m_WindowSurface->w / 2,  (float)m_WindowSurface->h / 2,0 };
	MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	map->AddComponent(mapComp);
	map->AddWatcher(new LevelCompleteObserver());
	scene.AddMap(map);
	
	{
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer1");
			scoreDisplay->AddComponent(new TransformComponent({ 200,0,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new dae::TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);

			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer1");
			livesDisplay->AddComponent(new TransformComponent());
			livesDisplay->SetPosition(400, 0,10);

			font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer2");
			scoreDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w - 200.0f,0.0f,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new dae::TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);

			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer2");
			livesDisplay->AddComponent(new TransformComponent());
			livesDisplay->SetPosition(m_WindowSurface->w - 400.0f, 0,10);

			font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}

		{
			auto player = std::make_shared<GameObject>("Player1");

			const dae::Vector2 playerHalfSize = { 8,8 };
			const dae::Vector3 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) - 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,6 };

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
			const dae::Vector3 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) + 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,6 };
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
	{
		auto playerDiedPopup = std::make_shared<GameObject>("PlayerDiedPopup");
		playerDiedPopup->AddComponent(new TransformComponent());
		playerDiedPopup->AddComponent(new TimerComponent());
		playerDiedPopup->SetEnabled(false);
		playerDiedPopup->SetPosition(400, 400,10);
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto textComp = new dae::TextComponent("Player died", font);
		playerDiedPopup->AddComponent(textComp);

		scene.Add(playerDiedPopup);
	}
}

void Minigin::LoadVersusScene() const
{

	auto& scene = SceneManager::GetInstance().CreateScene("VersusScene", (int)GameMode::Versus);

	auto map = std::make_shared<GameObject>("Map");
	//250,200
	const dae::Vector3 highestCubePos = { (float)m_WindowSurface->w / 2,  (float)m_WindowSurface->h / 2,0 };
	MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	map->AddComponent(mapComp);
	map->AddWatcher(new LevelCompleteObserver());
	scene.AddMap(map);

	{
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer1");
			scoreDisplay->AddComponent(new TransformComponent({ 200,0,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new dae::TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);

			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer1");
			livesDisplay->AddComponent(new TransformComponent());
			livesDisplay->SetPosition(400, 0, 10);

			font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}
		{
			auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplayPlayer2");
			scoreDisplay->AddComponent(new TransformComponent({ m_WindowSurface->w - 200.0f,0.0f,10 }, 1.0f));

			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto scoreCounter = new dae::TextComponent("Score: 0", font);
			scoreDisplay->AddComponent(scoreCounter);

			scene.Add(scoreDisplay);

			auto livesDisplay = std::make_shared<GameObject>("LivesDisplayPlayer2");
			livesDisplay->AddComponent(new TransformComponent());
			livesDisplay->SetPosition(m_WindowSurface->w - 400.0f, 0, 10);

			font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
			auto livesCounter = new dae::TextComponent("Remaining lives: 3", font);
			livesDisplay->AddComponent(livesCounter);

			scene.Add(livesDisplay);
		}

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
		{
			auto coily = std::make_shared<GameObject>("Coily");

			SDL_Rect srcRect = { 0,32,16,32 };
			const dae::Vector2 srcRectHalf = { 8,16 };
			coily->AddComponent(new RenderComponent(srcRect));
			coily->SetTexture("Textures/Qbert2.png");

			const Vector3 pos = { (m_WindowSurface->w / 2 + srcRectHalf.x) + 96, (m_WindowSurface->h / 2 - srcRectHalf.y) + 144,6 };

			coily->AddComponent(new TransformComponent(pos, 1.0f));

			coily->AddComponent(new HealthComponent(3));
			coily->AddComponent(new MoveComponent(6));
			coily->AddComponent(new ControlComponent(pos));
			coily->AddTag(dae::Tag::Coily);
			CollisionManager::GetInstance().AddCollider(coily);

			scene.AddPlayer(coily);
		}
	}
	{
		auto playerDiedPopup = std::make_shared<GameObject>("PlayerDiedPopup");
		playerDiedPopup->AddComponent(new TransformComponent());
		playerDiedPopup->AddComponent(new TimerComponent());
		playerDiedPopup->SetEnabled(false);
		playerDiedPopup->SetPosition(400, 400, 10);
		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		auto textComp = new dae::TextComponent("Player died", font);
		playerDiedPopup->AddComponent(textComp);

		scene.Add(playerDiedPopup);
	}
}


void Minigin::LoadGame() const
{
	
	LoadVersusScene();

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
			enemyManager.Update(deltaTime);
			collisionManager.Update(deltaTime);
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