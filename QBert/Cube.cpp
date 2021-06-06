#include "MiniginPCH.h"
#include "Cube.h"
#include <memory>
#include "RenderComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MapComponent.h"

using namespace dae;

Cube::Cube(int id)
	:m_Id{ id }
	, m_Activated{ false }
	, m_CurrentColor{ CubeColor::Red }
	, m_IntermediateActivated{ false }
{
	m_pGameObject = std::make_shared<GameObject>(("Cube" + std::to_string(id)));
}

bool Cube::SetActivated(bool activate)
{
	MapComponent* CurrentMap = SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>();

	if (CurrentMap->GetCurrentLevel() == MapComponent::Level::LevelOne)
	{
		if (activate && !m_Activated)
		{
			m_Activated = activate;
			SetColorNext(activate);
			SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->Notify(dae::Event::ColorChanged);
			return true;
		}
		else if (!activate && m_Activated)
		{
			m_Activated = activate;
			SetColorNext(activate);
		}
	}
	else if (CurrentMap->GetCurrentLevel() == MapComponent::Level::LevelTwo)
	{
		if (!m_IntermediateActivated && activate)
		{
			m_IntermediateActivated = true;
			SetColorNext(activate);
		}
		else if (activate && !m_Activated && m_IntermediateActivated)
		{
			m_Activated = activate;
			SetColorNext(activate);
			SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->Notify(dae::Event::ColorChanged);
			return true;
		}
		else if (!activate && m_IntermediateActivated)
		{
			m_Activated = activate;
			m_IntermediateActivated = activate;
			SetColorNext(activate);
		}
		else if (!activate && m_Activated)
		{
			m_Activated = activate;
			m_IntermediateActivated = true;
			SetColorNext(activate);
		}
	}
	else if (CurrentMap->GetCurrentLevel() == MapComponent::Level::LevelThree)
	{
		if (activate)
		{
			m_Activated = !m_Activated;

			if (m_Activated)
			{
				SetColorNext(true);
				SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->Notify(dae::Event::ColorChanged);
				return true;
			}

			else if (!m_Activated)
				SetColorNext(false);

		}
		else if (!activate && m_Activated)
		{
			m_Activated = activate;
			SetColorNext(activate);
		}
	}

	return false;
}

void Cube::SetColorNext(bool next) // next or previous
{
	if (next)
	{
		if (m_CurrentColor == CubeColor::Blue)
			SetColor(CubeColor::Red);
		else
			SetColor(CubeColor((int)m_CurrentColor + 1));
	}
	else
	{
		if (m_CurrentColor == CubeColor::Red)
			SetColor(CubeColor::Blue);
		else
			SetColor(CubeColor((int)m_CurrentColor - 1));
	}

}
void Cube::SetColor(const CubeColor& color)
{
	m_CurrentColor = color;
	const int redTileSrcRectY = 160;
	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	SDL_Rect srcRect = renderComp->GetSrcRect();
	srcRect.y = redTileSrcRectY + srcRect.h * (int)color;
	renderComp->SetSrcRect(srcRect);
}

void Cube::Update(float deltaT)
{
	m_pGameObject->Update(deltaT);
}

void Cube::Render() const
{
	m_pGameObject->Render();
}
