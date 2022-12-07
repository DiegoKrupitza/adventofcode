#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cassert>
#include <cctype>
#include <algorithm>
#include <numeric>

char findDuplicate(std::string &line) 
{
    // there are only 52 chars in total but since in ascii between upper 
    // and lower we have 6 special chars and we dont want to make extra calcs 
    bool letterCounts [58] = {false};

    int middle = line.length()/2;
    
    std::set<char> leftSet(line.begin(), line.begin() + middle);
    std::set<char> rightSet(line.begin() + middle, line.end());

    for(const auto& character : leftSet) 
    {
        letterCounts[character-65] = true;
    }    

    for(const auto& character : rightSet) 
    {
        if(letterCounts[character-65]) 
        {
            return character;
        }
    }    

    // according to the problem description this should never happen!
    assert(false);
    return 0;
}

int main() 
{

    std::vector<char> duplicates{};

    for(std::string line;std::getline(std::cin,line);) 
    {
        char dup = findDuplicate(line);
        duplicates.push_back(dup);
    }

    std::vector<int> prios;
    
    std::transform(
        duplicates.begin(),
        duplicates.end(),
        std::back_inserter(prios),
        [](auto& character) {
            int subtractor = (std::islower(character)) ? 96 : 38;
            return int(character) - subtractor;
        }
    );

    int sum = std::accumulate(prios.begin(),prios.end(),decltype(prios)::value_type(0));

    std::cout << "Sum: " << sum << std::endl;
    return 0;
}