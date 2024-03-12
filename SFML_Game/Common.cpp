#include "Common.h"

// RandomNumberGenerator
//------------------------------------------------------------------------------------------------------------
float RandomNumberGenerator::getRandom(int start, int end)
{
    if (start < end)
    {
        long long null_number = std::chrono::system_clock::now().time_since_epoch().count();

        std::default_random_engine rnd = std::default_random_engine(static_cast<unsigned int>(null_number));
        std::uniform_int_distribution<int> distr(start, end);

        return static_cast<float>(distr(rnd));
    }
    return 0;
}
//------------------------------------------------------------------------------------------------------------




// FileHandler
//------------------------------------------------------------------------------------------------------------
std::string FileHandler::readFile(std::string filePath, const unsigned int lineNumber)
{
    std::ifstream file(filePath);
    std::string line;

    if (!file.is_open())
    {
        return "Error opening file";
    }

    for (size_t i = 0; i < lineNumber; i++)
    {
        if (!std::getline(file, line))
        {
            file.close();
            return "0";
        }
    }

    file.close();
    return line.empty() ? "0" : line;
}
//------------------------------------------------------------------------------------------------------------
void FileHandler::writeFile(std::string filePath, std::string info, const unsigned int lineNumber)
{
    std::ifstream fileIn(filePath);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(fileIn, line))
    {
        lines.push_back(line);
    }
    fileIn.close();

    if (lineNumber > lines.size())
    {
        lines.resize(lineNumber, "");
    }

    lines[lineNumber - 1] = info;

    std::ofstream fileOut(filePath, std::ios::trunc);

    for (const auto& outLine : lines)
    {
        fileOut << outLine << std::endl;
    }

    fileOut.close();
}
//------------------------------------------------------------------------------------------------------------