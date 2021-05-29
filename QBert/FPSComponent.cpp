#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
using namespace dae;

FPSComponent::FPSComponent()
{
}

FPSComponent::FPSComponent(TextComponent* textComp)
	:m_pTextComponent{textComp}
{
}
void FPSComponent::Update(float deltaT)
{
	m_Fps = 1.0f / deltaT;

	if (m_pTextComponent)
		m_pTextComponent->SetText("FPS: " + std::to_string(m_Fps));
}

std::string FPSComponent::GetFPSAsString() const
{
	return std::to_string(m_Fps);
}