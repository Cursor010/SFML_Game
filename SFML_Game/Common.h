#pragma once

#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <array>
#include <SFML/Graphics.hpp>

//------------------------------------------------------------------------------------------------------------
struct RandomNumberGenerator
{
    float getRandom(int start, int end);
};
//------------------------------------------------------------------------------------------------------------
struct FileHandler
{
    std::string readFile(std::string filePath, const unsigned int lineNumber);
    void writeFile(std::string filePath, std::string info, const unsigned int lineNumber);
};
//------------------------------------------------------------------------------------------------------------