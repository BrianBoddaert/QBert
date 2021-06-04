#include "MiniginPCH.h"
#include "TxtLoader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TxtLoader::TxtLoader(std::string filePath)
    :m_FilePath{ filePath }
{
}

std::vector<dae::Vector2> TxtLoader::ReadVec3()
{
    std::ifstream input;
    std::string currentLine;
    std::string stringBuffer;
    float bufferX = 0;
    float bufferY = 0;
    std::vector<dae::Vector2> positions;

    input.open(m_FilePath, std::ios::in | std::ios::binary);
    if (input.is_open())
    {
        while (std::getline(input, currentLine))
        {
            for (size_t i = 0; i < currentLine.size(); i++)
            {
                if (currentLine[i] != ',' && currentLine[i] != '\r')
                {
                    stringBuffer += currentLine[i];
                }
                else if (currentLine[i] == ',')
                {
                    bufferX = std::stof(stringBuffer);
                    stringBuffer = "";
                }
            }
            bufferY = std::stof(stringBuffer);
            stringBuffer = "";
            positions.push_back(dae::Vector3{ bufferX, bufferY, 0 });
        }
    }

    return positions;
}