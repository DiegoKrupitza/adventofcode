#include <iostream>
#include <vector>
#include <numeric>
#include <sstream>
#include <string>

#define LOGGING_ENABLED false

#if LOGGING_ENABLED == true
#define LOG(s) std::cout << s << std::endl
#else
#define LOG(s)
#endif

// split method from here: https://www.reddit.com/r/cpp/comments/5dxnwm/comment/da8ewky/?utm_source=share&utm_medium=web2x&context=3
std::vector <std::string> split(const std::string &s, const char delimiter) {
    std::vector <std::string> result;

    std::size_t current = 0;
    std::size_t p = s.find_first_of(delimiter, 0);

    while (p != std::string::npos) {
        // emplace_back will call the constructor of std::string
        result.emplace_back(s, current, p - current);
        current = p + 1;
        p = s.find_first_of(delimiter, current);
    }

    result.emplace_back(s, current);
    return result;
}

enum class InstructionCode : char {
    NOOP = 'N',
    ADDX = 'A'
};

struct CPUInstruction {
public:
    InstructionCode code;
    int value;

    CPUInstruction(InstructionCode code, int value) : code(code), value(value) {}

    CPUInstruction static parseInstruction(std::string &line) {
        const auto splitted = split(line, ' ');
        if (splitted[0] == "noop") {
            return {InstructionCode::NOOP, 0};
        }
        return {InstructionCode::ADDX, std::stoi(splitted[1])};
    }
};

std::ostream &operator<<(std::ostream &os, const CPUInstruction &ins) {
    os << static_cast<char>(ins.code);

    if (ins.code != InstructionCode::NOOP) {
        os << " " << ins.value;
    }
    return os;
}

std::vector <CPUInstruction> getInstructionsFromStdIn() {
    std::vector <CPUInstruction> result{};
    for (std::string line; std::getline(std::cin, line);) {
        result.push_back(CPUInstruction::parseInstruction(line));
    }
    return result;
}

int main() {
    int X = 1;
    bool waitCycle = false;

    std::stringstream result;

    const auto instructions = getInstructionsFromStdIn();

    int cycle = 1;
    for (size_t index = 0; index < instructions.size(); cycle++) {
        // start cycle
        LOG("===");
        LOG("Start Cycle: " << cycle);

        // during cycle 
        LOG("During cycle " << cycle << ": CRT draws pixel in position " << cycle-1);

        const auto cycleCleaned = cycle-1 % 39;
        const auto currentSymbol = (cycleCleaned == X || cycleCleaned == X-1 || cycleCleaned == X+1)? '#': '.';
        result << currentSymbol;

        if(cycle % 40 == 0) 
        {
            result << '\n';
            X += 40; // we have to increment by 40 since all the cycles in the new line will be 40 ahead
        }

        LOG("Current symbol is: " << currentSymbol); 

        const auto &currentIns = instructions[index];
        if (currentIns.code == InstructionCode::ADDX && !waitCycle) {
            waitCycle = true;
            continue;
        }

        // end cycle
        X += currentIns.value;

        waitCycle = false;
        index++;

        LOG("End of cycle  " << cycle << ": finish executing " << currentIns << " (Register X is now " << X << ")");
    }

    std::cout << result.str();

    return 0;
}