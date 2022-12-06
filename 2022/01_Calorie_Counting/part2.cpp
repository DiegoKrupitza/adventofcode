#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

int main() {

    int currentSum = 0;  
    std::vector<int> calories {};

    for(std::string line; std::getline(std::cin, line);) {
        if(line.empty()) {
            // we will process the next elf
            calories.push_back(currentSum);
            std::push_heap(calories.begin(),calories.end());

            currentSum = 0;
            continue;
        }
        currentSum += std::stoi(line);
    }

    int result = 0;
    for(int i = 0;i < 3;i++) {
        std::pop_heap(calories.begin(),calories.end());
        result += calories.back();
        calories.pop_back();
    }

    std::cout << "Result: " << result << std::endl;
    
    return 0;
}