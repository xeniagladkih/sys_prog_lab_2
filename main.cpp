#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <fstream>

class NFA {
public:
    // Клас для представлення переходів між станами
    struct Transition {
        char symbol;  // Символ переходу
        int nextState; // Наступний стан
    };

    // Додамо структуру для представлення стану автомата
    struct State {
        int stateID; // Ідентифікатор стану
        std::vector<Transition> transitions; // Масив переходів
    };

    // Конструктор класу
    NFA(int initialState, const std::set<int>& acceptingStates)
            : initialState(initialState), acceptingStates(acceptingStates) {}

    // Функція для додавання переходу
    void addTransition(int fromState, char symbol, int toState) {
        states[fromState].transitions.push_back({symbol, toState});
    }

    // Функція для перевірки чи приймає автомат вхідний рядок
    bool accept(const std::string& input) {
        std::set<int> currentStates = {initialState};
        std::set<int> nextStates;

        for (char symbol : input) {
            for (int state : currentStates) {
                for (const auto& transition : states[state].transitions) {
                    if (transition.symbol == symbol) {
                        nextStates.insert(transition.nextState);
                    }
                }
            }

            currentStates = nextStates;
            nextStates.clear();
        }

        // Перевіряємо, чи є хоча б один приймаючий стан в кінці
        for (int state : currentStates) {
            if (acceptingStates.count(state) > 0) {
                return true;
            }
        }

        return false;
    }

private:
    int initialState; // Початковий стан
    std::set<int> acceptingStates; // Множина приймаючих станів
    std::unordered_map<int, State> states; // Словник станів автомата
};

int main() {
    NFA nfa(0, {0,1,2,3}); // Початковий стан 0, приймаючий стани 0,1,2,3

    // Додамо переходи
    nfa.addTransition(0, 'a', 1);
    nfa.addTransition(0, 'b', 2);
    nfa.addTransition(0, 'c', 3);

    nfa.addTransition(1, 'a', 0);
    nfa.addTransition(1, 'b', 2);
    nfa.addTransition(1, 'c', 3);

    nfa.addTransition(2, 'a', 1);
    nfa.addTransition(2, 'b', 0);
    nfa.addTransition(2, 'c', 3);

    nfa.addTransition(3, 'a', 1);
    nfa.addTransition(3, 'b', 2);
    nfa.addTransition(3, 'c', 3);

    // Масив посилань на файли
    std::vector<std::ifstream> inputFiles;

    // Додаємо тестові файли
    inputFiles.emplace_back(std::ifstream("../test1.txt"));
    inputFiles.emplace_back(std::ifstream("../test2.txt"));

    // Перевірка кожного файла
    for (std::ifstream& inputFile : inputFiles) {
        if (!inputFile.is_open()) {
            std::cerr << "Unable to open file!" << std::endl;
            return 1; // Помилка відкриття файлу
        }

        std::string line;

        int totalTests = 0;
        int passedTests = 0;
        while (std::getline(inputFile, line)) {
            totalTests++;

            // Перевірка чи приймає автомат певний рядок
            bool result = nfa.accept(line);
            std::cout << line << ": " << (result ? "Accepted" : "Rejected") << std::endl;

            if (result) {
                passedTests++;
            }
        }

        std::cout << "Passed tests for file: " << passedTests << " out of " << totalTests << std::endl;

        // Закриття файла
        inputFile.close();
    }
    return 0;
}