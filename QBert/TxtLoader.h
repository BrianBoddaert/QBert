#pragma once
#include <glm\vec3.hpp>
#include <vector>
#include "Structs.h"

class TxtLoader
{
public:
    TxtLoader(std::string filePath);
    ~TxtLoader() = default;

    TxtLoader(const TxtLoader&) = delete;
    TxtLoader(TxtLoader&&) noexcept = delete;
    TxtLoader& operator=(const TxtLoader&) = delete;
    TxtLoader& operator=(TxtLoader&&) noexcept = delete;

    std::vector<dae::Vector2> ReadVec3();

private:
    std::string m_FilePath;
};