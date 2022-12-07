#include <iostream>
#include <string>
#include <unordered_map>

/*
Letter meaning:
A - Rock 
B - Paper
C - Scissors

X - Lose 
Y - Draw
Z - WIN

Shape value: 
ROCK     = A => 1
PAPER    = B => 2
SCISSORS = C => 3

Round value:
Lose - 0
Draw - 3
Win  - 6

Input: 
A X

Rules:
Rock defeats Scissors
Scissors defeats Paper
Paper defeats Rock
*/

// key = opponent move 
// value = my move 
std::unordered_map<int, int> loseMap {{1,3},{3,2},{2,1}};
std::unordered_map<int, int> winMap {{1,2},{2,3},{3,1}};


int convertToRPS(const char& input) 
{
    if (input == 'A') 
    {
        return 1;
    }
    else if (input == 'B') 
    {
        return 2;
    }
    
    return 3;    
}

int roundResult(const char& resultChar) 
{
    if (resultChar == 'X') 
    {
        return 0;
    }
    else if (resultChar == 'Y') 
    {
        return 3;
    }
    
    return 6;   
}

int calcCorrectMove(const int &opponentMove, const char &resultChar) 
{
    if (resultChar == 'Y') 
    {
        return opponentMove;
    }
    else if(resultChar == 'X')
    {
        return loseMap[opponentMove];
    }
    else 
    {
        return winMap[opponentMove];
    }
    
}

int main() 
{
    int total = 0;

    for(std::string entry; std::getline(std::cin,entry);) 
    {
        if (entry.empty()) 
        {
            break;
        }
        int opponentMove = convertToRPS(entry[0]);
        char resultChar = entry[2];

        int myMove = calcCorrectMove(opponentMove,resultChar);
        int result = roundResult(resultChar);
        
        total += (result + myMove);
    }

    std::cout << "Total: " << total << std::endl;

    return 0;
}