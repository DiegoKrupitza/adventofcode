#include <iostream>
#include <vector>
#include <numeric>

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

    const auto instructions = getInstructionsFromStdIn();

    std::vector <std::pair<int, int>> signalStrengths;

    for (size_t cycle = 1, index = 0; index < instructions.size(); cycle++) {
        // start cycle
        LOG("===");
        LOG("Cycle: " << cycle);

        // during cycle 
        LOG("During Cycle value is: " << X);
        if (cycle == 20 || (cycle > 40 && (cycle - 20) % 40 == 0)) {
            signalStrengths.push_back({cycle, X});
        }

        const auto &currentIns = instructions[index];
        if (currentIns.code == InstructionCode::ADDX && !waitCycle) {
            waitCycle = true;
            continue;
        }

        // end cycle
        LOG("Executing: " << currentIns);
        X += currentIns.value;

        waitCycle = false;
        index++;

        LOG("Value of X at the end of cycle [" << cycle << "] is: " << X);
    }

#if LOGGING_ENABLED == true
    for (const auto& sig: signalStrengths) 
    {
        LOG(sig.first << " " << sig.second <<" " << sig.first*sig.second);
    }
#endif

    int result = std::accumulate(signalStrengths.begin(), signalStrengths.end(), 0,
                                 [](int v, const std::pair<int, int> &entry) {
                                     return v + (entry.first * entry.second);
                                 });

    std::cout << "Sum of the special signals is: " << result << std::endl;

    return 0;
}