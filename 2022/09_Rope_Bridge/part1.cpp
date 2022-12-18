#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <cmath>

#define LOGGING false

#if LOGGING == true
#define LOG(s) std::cout << s << std::endl
#else
#define LOG(s)
#endif

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

enum class MoveDirection: char
{
    LEFT = 'L',
    RIGHT = 'R',
    UP = 'U',
    DOWN = 'D'
};

std::ostream& operator<<(std::ostream& os, const MoveDirection& d) 
{
    os << static_cast<char>(d);
    return os;
}

struct MoveInstruction
{
    MoveDirection direction;
    int steps;

    MoveInstruction(MoveDirection direction, int steps) : direction(direction), steps(steps) 
    {
    }

    
    static std::vector<MoveInstruction> parseFromLine(const std::string &line) 
    {
        std::vector<MoveInstruction> instructions;
        const auto splitted = split(line,' ');
        const auto steps = std::stoi(splitted[1]);

        for(int i = 0; i < steps; i++) 
        {
            instructions.push_back({
                static_cast<MoveDirection>(splitted[0][0]),
                1
            });
        }

        return instructions;
    }

};

std::ostream& operator<<(std::ostream& os, const MoveInstruction& mI)
{
    os << mI.direction << " " << mI.steps;
    return os;
}

std::vector<MoveInstruction> getInstructionsFromStdin()
{
    std::vector<MoveInstruction> moveInstructions{};

    for(std::string line; std::getline(std::cin,line);)
    {
        const auto parseSubinstructions = MoveInstruction::parseFromLine(line);
        moveInstructions.insert(moveInstructions.end(), parseSubinstructions.begin(),parseSubinstructions.end());
    }

    return moveInstructions;
}

namespace std {
    std::ostream& operator<<(std::ostream& os, const std::pair<int,int> &p)
    {
        os << "X=" << p.first << ",Y=" << p.second;
        return os;
    }
}

void moveByInstruction(const MoveInstruction &instruction,std::pair<int,int> &hPosition)
{
    // first is x
    // second is y
    if(instruction.direction == MoveDirection::LEFT)
    {
        hPosition.first--;
    }
    else if (instruction.direction == MoveDirection::RIGHT)
    {
        hPosition.first++;
    }
    else if (instruction.direction == MoveDirection::UP)
    {
        hPosition.second++;
    }else{
        hPosition.second--;
    }
}

inline float getDistance(const std::pair<int,int> &p1,const std::pair<int,int> &p2)
{
    return std::sqrt(std::pow(p1.first - p2.first,2)+std::pow(p1.second - p2.second,2));
}

int main()
{

    const auto moveInstructions = getInstructionsFromStdin();

    // hPosition[x,y]  tPosition[x,y]
    std::pair<int,int> hPosition {0,0};
    std::pair<int,int> tPosition {0,0};

    std::set<std::pair<int,int>> visited{tPosition};
    
    for(const auto& instruction: moveInstructions) 
    {
        LOG("Current hPositions: " << hPosition);
        LOG("Current tPositions: " << tPosition);
        LOG("Processing: " << instruction);
        
        // make move for the head
        moveByInstruction(instruction,hPosition);

        // check if tail has to move
        const auto distance = getDistance(hPosition,tPosition);
        if(distance >= 2) 
        {
            if(tPosition.first == hPosition.first || tPosition.second == hPosition.second) 
            {
                // same row col therefore no dig move
                moveByInstruction(instruction,tPosition);
            }
            else if(instruction.direction == MoveDirection::LEFT) 
            {
                tPosition.first--;
                tPosition.second = hPosition.second;
            }
            else if(instruction.direction == MoveDirection::RIGHT) 
            {
                tPosition.first++;
                tPosition.second = hPosition.second;
            }
            else if(instruction.direction == MoveDirection::UP) 
            {
                tPosition.first = hPosition.first;
                tPosition.second++;
            }
            else if(instruction.direction == MoveDirection::DOWN) 
            {
                tPosition.first = hPosition.first;
                tPosition.second--;
            }
        }
        
        visited.insert({tPosition});

        LOG("==========");
    }

    LOG("FINAL POS:");
    LOG("hPosition: " << hPosition);
    LOG("tPosition: " << tPosition);

    std::cout << "Number of places visited: " << visited.size() << std::endl;

    return 0;
}