#pragma once
#include "Component.h"
#include <memory>


namespace dae
{
    class TextComponent;

    class FPSComponent :
        public Component
    {
    public:
        FPSComponent();
        FPSComponent(TextComponent*);
        void Update(float deltaT) override;
        std::string GetFPSAsString() const;
    private:
        float m_Fps;
        TextComponent* m_pTextComponent = nullptr; // OPTIONAL DISPLAY
    };

}