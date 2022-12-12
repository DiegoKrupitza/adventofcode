#include <iostream>
#include <string>
#include <unordered_map>

void removeEntries(const std::string &input,std::unordered_map<char,std::string::size_type> &found_chars, const std::string::size_type start,const std::string::size_type end) 
{
    for(auto i = start; i <= end; i++) 
    {
        found_chars.erase(input[i]);
    }
}

int findStartOfPacket(const std::string &input)
{
    std::string::size_type leftPointer = 0;
    std::string::size_type rightPointer = 0;

    std::unordered_map<char,std::string::size_type> found_chars {};

    while(rightPointer < input.length() &&  found_chars.size() < 14)
    {
        auto current_right_char = input[rightPointer];
        auto found_location = found_chars.find(current_right_char);

        if(found_location != found_chars.end()) 
        {
            auto old_location = found_location->second;
            
            // remove all the chars from left till found_location->second (inclusive)
            removeEntries(input,found_chars, leftPointer,found_location->second);
            leftPointer = (old_location) + 1;
        }

        found_chars.insert({current_right_char,rightPointer});
        rightPointer++;

    }

    return rightPointer;
}

int main()
{
    for(std::string input; std::getline(std::cin,input);)
    {
        std::cout << "===== " << input << std::endl;
        auto result = findStartOfPacket(input);
        std::cout << "Start of Packet: " << result << std::endl;
    }

    return 0;
}