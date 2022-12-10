#include <iostream>
#include <string>
#include <vector>
#include <chrono>

// split method from here: https://www.reddit.com/r/cpp/comments/5dxnwm/comment/da8ewky/?utm_source=share&utm_medium=web2x&context=3
std::vector<std::string> split(const std::string& s, const char delimiter) 
{
    std::vector<std::string> result;

    std::size_t current = 0;
    std::size_t p = s.find_first_of(delimiter, 0);

    while (p != std::string::npos)
    {
        // emplace_back will call the constructor of std::string
        result.emplace_back(s, current, p - current);
        current = p + 1;
        p = s.find_first_of(delimiter, current);
    }

    result.emplace_back(s, current);
    return result;
}

int isOverLapped(const std::vector<std::string> &elf1Fields,const std::vector<std::string> &elf2Fields) 
{
    auto elf1Start = std::stoi(elf1Fields[0]);
    auto elf1End = std::stoi(elf1Fields[1]);

    auto elf2Start = std::stoi(elf2Fields[0]);
    auto elf2End = std::stoi(elf2Fields[1]);
    
    return ((elf1Start <= elf2Start) && (elf1End >= elf2Start)) || (elf1Start <= elf2End && elf1End >= elf2End) || ((elf1Start <= elf2Start) && (elf1End >= elf2End)) ||
    ((elf2Start <= elf1Start) && (elf2End >= elf1Start)) || (elf2Start <= elf1End && elf2End >= elf1End) || ((elf2Start <= elf1Start) && (elf2End >= elf1End));
}

int main() 
{
    int fullyContained = 0;

    for(std::string line; std::getline(std::cin,line);)
    {
        auto regions = split(line,',');
        auto elf1Fields = split(regions[0],'-');
        auto elf2Fields = split(regions[1],'-');

        fullyContained += isOverLapped(elf1Fields,elf2Fields);
    }

    std::cout << "Fully contained: " << fullyContained << std::endl;

    return 0;
}