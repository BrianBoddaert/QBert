#pragma once
#include "GameObject.h"
#include "Scene.h"
#include "Structs.h"
#include "TransformComponent.h"

class Disc
{
public:
    Disc(const dae::Vector3& finalPos, const dae::Vector2& srcRect);

    Disc(const Disc& other) = delete;
    Disc(Disc&& other) = delete;
    Disc& operator=(const Disc& other) = delete;
    Disc& operator=(Disc&& other) = delete;
    ~Disc() = default;

    void Update(float deltaT);
    void Render() const;

    std::shared_ptr<dae::GameObject> GetGameObject() const { return m_pGameObject; };

    void SetMoving(bool isMoving) { m_Moving = isMoving; }

    bool GetMarkedForDelete() { return m_MarkedForDelete; }
    void SetMarkedForDelete(bool v) { m_MarkedForDelete = v; }
private:
    void Elevate(float deltaT);

    std::shared_ptr<dae::GameObject> m_pGameObject = nullptr;
    dae::TransformComponent* m_pTransformComponent = nullptr;

    float m_AnimTimer = 0;
    float m_AnimTime = 0.2f;

    float m_MoveFactor = 0.f;
    float m_Speed = 150.f;

    dae::Vector3 m_FinalPos;
    dae::Vector3 m_Direction = dae::Vector3{ 0,0,0 };

    bool m_Moving = false;
    bool m_MarkedForDelete = false;

};