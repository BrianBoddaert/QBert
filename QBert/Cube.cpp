#include "MiniginPCH.h"
#include "Cube.h"
#include <memory>
#include "RenderComponent.h"
#include "SceneManager.h"
#include "Scene.h"
using namespace dae;

Cube::Cube(int id)
	:m_Id{ id }
	, m_Activated{ false }
	, m_CurrentColor{ CubeColor::Red }
{
	m_pGameObject = std::make_shared<GameObject>(("Cube" + std::to_string(id)));
}

void Cube::SetActivated(bool activate)
{
	if (activate && !m_Activated)
	{
		m_Activated = activate;
		SetColorNext(activate);
		SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->Notify(dae::Event::ColorChanged);
	}
	else if (!activate && m_Activated)
	{
		m_Activated = activate;
		SetColorNext(activate);
	}






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
