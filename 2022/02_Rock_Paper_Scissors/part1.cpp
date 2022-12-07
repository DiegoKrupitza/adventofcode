#include <iostream>
#include <string>

/*
Letter meaning:
A - Rock 
B - Paper
C - Scissors

X - Rock 
Y - Paper
Z - Scissors

Shape value: 
ROCK     = A,X => 1
PAPER    = B,Y => 2
SCISSORS = C,Z => 3

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

int convertToRPS(const char& input) 
{
    if (input == 'A' || input == 'X') 
    {
        return 1;
    }
    else if (input == 'B' || input == 'Y') 
    {
        return 2;
    }
    
    return 3;    
}

int calculateRoundResult(int &opponentMove, int &myMove) 
{
    if (opponentMove == myMove) {
        return 3;
    }
    else if ((opponentMove == 1 && myMove == 3) || (opponentMove == 3 && myMove == 2) || (opponentMove == 2 && myMove == 1)) 
    {
        return 0;
    }
    return 6;
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
        int myMove = convertToRPS(entry[2]);

        int result = calculateRoundResult(opponentMove,myMove);

        total += (result + myMove);
    }

    std::cout << "Total: " << total << std::endl;

    return 0;
}