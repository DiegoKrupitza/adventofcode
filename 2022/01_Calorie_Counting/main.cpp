#include <iostream>
#include <string>


int main() {

    int maximumCarried = -1;
    int maxElfId = -1;

    int currentSum = 0;   
    int currentElf = 1;

    for(std::string line; std::getline(std::cin, line);) {
        if(line.empty()) {
            // we will process the next elf
            if (currentSum >= maximumCarried) {
                maximumCarried = currentSum;
                maxElfId = currentElf;
            }
            currentElf++;
            currentSum = 0;
            continue;
        }
        currentSum += std::stoi(line);
    }

    std::cout << "ELF: " << maxElfId << "\tSum: " << maximumCarried << std::endl;
    return 0;
}