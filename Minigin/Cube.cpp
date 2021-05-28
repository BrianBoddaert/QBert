#include "MiniginPCH.h"
#include "Cube.h"
#include <memory>
#include "RenderComponent.h"

using namespace dae;

Cube::Cube(int id)
	:m_Id{id}
	, m_Colored{ false }
{
	m_pGameObject = std::make_shared<GameObject>(("Cube" + std::to_string(id)));
}

void Cube::Color()
{
	
	if (!m_Colored)
	{
		m_Colored = true;
		RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
		SDL_Rect srcRect = renderComp->GetSrcRect();
		srcRect.y += srcRect.h;
		renderComp->SetSrcRect(srcRect);
	}
		

}

void Cube::Update(float deltaT)
{
	m_pGameObject->Update(deltaT);
}

void Cube::Render() const
{
	m_pGameObject->Render();
}
