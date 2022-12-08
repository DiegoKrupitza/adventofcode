#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cassert>
#include <cctype>
#include <algorithm>
#include <numeric>

char findBadge(std::string &elf1,std::string &elf2,std::string &elf3) 
{
    // there are only 52 chars in total but since in ascii between upper 
    // and lower we have 6 special chars and we dont want to make extra calcs 
    int letterCounts [58] = {0};

    
    std::set<char> elf1Set(elf1.begin(), elf1.end());
    std::set<char> elf2Set(elf2.begin(), elf2.end());
    std::set<char> elf3Set(elf3.begin(), elf3.end());

    for(const auto& character : elf1Set) 
    {
        letterCounts[character-65]++;
    }    

    for(const auto& character : elf2Set) 
    {
        letterCounts[character-65]++;
    }    

    for(const auto& character : elf3Set) 
    {
        letterCounts[character-65]++;
    }    

    for(int i = 0; i < *(&letterCounts + 1) - letterCounts; i++) 
    {
        if(letterCounts[i] == 3) {
            return char(i+65);
        }
    }

    // according to the problem description this should never happen!
    assert(false);
    return 0;
}

std::vector<int> calculatePriorities(const std::vector<char> &badges) 
{
    std::vector<int> prios;
    std::transform(
        badges.begin(),
        badges.end(),
        std::back_inserter(prios),
        [](auto& character) {
            int subtractor = (std::islower(character)) ? 96 : 38;
            return int(character) - subtractor;
        }
    );
    return prios;
}

int main() 
{
    std::vector<std::string> lines;
    
    // accumulate the lines
    for(std::string line;std::getline(std::cin,line);) 
    {
        lines.push_back(line);
    }

    // find the badges 
    std::vector<char> badges{};
    for(auto i = lines.begin(); i != lines.end(); i += 3) 
    {
        char badge = findBadge(*i,*(i+1),*(i+2));
        badges.push_back(badge);
    }
    
    std::vector<int> prios = calculatePriorities(badges);

    int sum = std::accumulate(prios.begin(),prios.end(),decltype(prios)::value_type(0));

    std::cout << "Sum: " << sum << std::endl;
    return 0;
}