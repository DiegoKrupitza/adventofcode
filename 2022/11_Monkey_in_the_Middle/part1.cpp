#include <iostream>
#include <vector>
#include <functional>
#include <list>
#include <utility>

#define LOGGING_ENABLED false
#if LOGGING_ENABLED == true
#define LOG(s) std::cout << s << std::endl
#else
#define LOG(s)
#endif

using ui64 = unsigned long long;

struct Monkey {
public:
    std::list <ui64> elements;
    std::function<ui64(ui64)> updateFunction;
    std::function<size_t(ui64)> forwardFunction;
    ui64 numberOfInspects = 0;

    Monkey(std::list <ui64> elements, std::function<ui64(ui64)> updateFunction,
           std::function<size_t(ui64)> forwardFunction) : elements(elements),
                                                          updateFunction(updateFunction),
                                                          forwardFunction(forwardFunction) {
    }

    std::vector <std::pair<size_t, ui64>> process() {
        std::vector <std::pair<size_t, ui64>> results;
        for (; !elements.empty(); elements.pop_front()) {
            numberOfInspects++;

            auto number = elements.front();

            number = updateFunction(number);
            number /= 3;

            results.push_back(
                    {
                            forwardFunction(number),
                            number
                    }
            );
        }
        return results;
    }

    void addEntry(ui64 element) {
        elements.push_back(element);
    }
};

std::vector <Monkey> getMonkeyInput() {
    const auto forwardFunctionGenerator =
            [](ui64 division, size_t forwardTrue, size_t forwardFalse) {
                return [division, forwardTrue, forwardFalse](ui64 value) {
                    return (value % division == 0) ? forwardTrue : forwardFalse;
                };
            };

    Monkey m0{{91, 66},
              [](const ui64 old) { return old * 13; },
              forwardFunctionGenerator(19, 6, 2)
    };
    Monkey m1{{78, 97, 59},
              [](const ui64 old) { return old + 7; },
              forwardFunctionGenerator(5, 0, 3)
    };
    Monkey m2{{57, 59, 97, 84, 72, 83, 56, 76},
              [](const ui64 old) { return old + 6; },
              forwardFunctionGenerator(11, 5, 7)
    };

    Monkey m3{{81, 78, 70, 58, 84},
              [](const ui64 old) { return old + 5; },
              forwardFunctionGenerator(17, 6, 0)
    };

    Monkey m4{{60},
              [](const ui64 old) { return old + 8; },
              forwardFunctionGenerator(7, 1, 3)
    };

    Monkey m5{{57, 69, 63, 75, 62, 77, 72},
              [](const ui64 old) { return old * 5; },
              forwardFunctionGenerator(13, 7, 4)
    };

    Monkey m6{{73, 66, 86, 79, 98, 87},
              [](const ui64 old) { return old * old; },
              forwardFunctionGenerator(3, 5, 2)
    };

    Monkey m7{{95, 89, 63, 67},
              [](const ui64 old) { return old + 2; },
              forwardFunctionGenerator(2, 1, 4)
    };

    return {m0, m1, m2, m3, m4, m5, m6, m7};
}

std::vector <Monkey> getMonkeyDemoInput() {
    const auto forwardFunctionGenerator = [](ui64 division, size_t forwardTrue, size_t forwardFalse) {
        return [division, forwardTrue, forwardFalse](ui64 value) {
            return (value % division == 0) ? forwardTrue : forwardFalse;
        };
    };


    Monkey m0{{79, 98},
              [](const ui64 old) { return old * 19; },
              forwardFunctionGenerator(23, 2, 3)
    };

    Monkey m1{{54, 65, 75, 74},
              [](const ui64 old) { return old + 6; },
              forwardFunctionGenerator(19, 2, 0)
    };

    Monkey m2{{79, 60, 97},
              [](const ui64 old) { return old * old; },
              forwardFunctionGenerator(13, 1, 3)
    };

    Monkey m3{{74},
              [](const ui64 old) { return old + 3; },
              forwardFunctionGenerator(17, 0, 1)
    };

    return {m0, m1, m2, m3};
}

int main() {

    auto monkeys = getMonkeyInput();

    for (int round = 1; round <= 20; round++) {
        LOG("Start Round: " << round);

        for (auto &monkey: monkeys) {
            const auto forwards = monkey.process();

            for (const auto &forward: forwards) {
                monkeys[forward.first].addEntry(forward.second);
            }
        }

#if LOGGING_ENABLED == true
        int count = 0;
        for(auto &monkey: monkeys)
        {
            std::cout << "Monkey " << count++ << ": {";
            std::for_each(monkey.elements.begin(), monkey.elements.end(), [](const ui64& value) {std::cout << value << ',';});
            std::cout << "}" << std::endl;
        }
#endif

        LOG("===Ended Round===");
    }

    ui64 max1 = 0;
    ui64 max2 = 0;

#if LOGGING_ENABLED == true
    size_t count = 0;
#endif
    for (auto &monkey: monkeys) {
        const auto &inspects = monkey.numberOfInspects;
        LOG("Inspects of Monkey " << count++ << ": " << inspects);

        if (inspects > max1) {
            max2 = max1;
            max1 = inspects;
        } else if (inspects > max2) {
            max2 = inspects;
        }
    }

    std::cout << "Solution is: " << max1 * max2 << std::endl;

    return 0;
}