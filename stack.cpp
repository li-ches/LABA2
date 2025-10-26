#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

// Хранение стека очков
struct BaseballGame {
    vector<int> scores;
};

// Функция для проверки, является ли строка числом
bool isNumber(const string& str) {
    if (str.empty()) return false;

    // Проверяем каждый символ
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Функция для обработки операций
int processOperations(const vector<string>& operations) {
    BaseballGame game;

    for (int i = 0; i < operations.size(); i++) {
        string op = operations[i];

        if (isNumber(op)) {
            // Преобразуем строку в число и добавляем в стек
            int score = stoi(op);
            if (score <= 0) {
                cout << "Ошибка: счет должен быть положительным числом. Получено: " << op << endl;
                return -1;
            }
            game.scores.push_back(score);
            cout << "Добавлен счет: " << score << endl;
        }
        else if (op == "+") {
            // Сумма двух предыдущих очков
            if (game.scores.size() < 2) {
                cout << "Ошибка: операция '+' требует как минимум два предыдущих счета" << endl;
                return -1;
            }
            int last = game.scores[game.scores.size() - 1];
            int secondLast = game.scores[game.scores.size() - 2];
            int newScore = last + secondLast;
            game.scores.push_back(newScore);
            cout << "Добавлена сумма: " << last << " + " << secondLast << " = " << newScore << endl;
        }
        else if (op == "D") {
            // Удвоение последнего счета
            if (game.scores.empty()) {
                cout << "Ошибка: операция 'D' требует как минимум один предыдущий счет" << endl;
                return -1;
            }
            int last = game.scores[game.scores.size() - 1];
            int newScore = last * 2;
            game.scores.push_back(newScore);
            cout << "Удвоен счет: " << last << " * 2 = " << newScore << endl;
        }
        else if (op == "C") {
            // Удаление последнего счета
            if (game.scores.empty()) {
                cout << "Ошибка: операция 'C' требует как минимум один предыдущий счет" << endl;
                return -1;
            }
            int removed = game.scores[game.scores.size() - 1];
            game.scores.pop_back();
            cout << "Удален счет: " << removed << endl;
        }
        else {
            cout << "Ошибка: неизвестная операция '" << op << "'" << endl;
            return -1;
        }

        // Вывод текущего состояния
        cout << "Текущие счета: ";
        if (game.scores.empty()) {
            cout << "пусто";
        } else {
            for (int j = 0; j < game.scores.size(); j++) {
                cout << game.scores[j];
                if (j < game.scores.size() - 1) cout << ", ";
            }
        }
        cout << endl << endl;
    }

    // Вычисляем сумму всех очков
    int total = 0;
    for (int score : game.scores) {
        total += score;
    }

    return total;
}

// Функция для ввода операций
vector<string> inputOperations() {
    vector<string> operations;
    string input;
    int count;

    cout << "Введите количество операций: ";
    cin >> count;

    if (count <= 0) {
        cout << "Количество операций должно быть положительным числом!" << endl;
        return operations;
    }

    cout << "Введите " << count << " операций:" << endl;
    cout << "(число - добавить счет, '+' - сумма двух предыдущих, 'D' - удвоить, 'C' - удалить)" << endl;

    for (int i = 0; i < count; i++) {
        cout << "Операция " << (i + 1) << ": ";
        cin >> input;
        operations.push_back(input);
    }

    return operations;
}

int main() {
    cout << "=== Учет очков в бейсбольной игре ===" << endl << endl;

    // Ввод операций от пользователя
    vector<string> operations = inputOperations();

    if (operations.empty()) {
        cout << "Нет операций для обработки." << endl;
        return 0;
    }

    cout << endl << "Обрабатываем операции..." << endl;
    cout << "=========================================" << endl;

    // Обработка операций
    int result = processOperations(operations);

    cout << "=========================================" << endl;

    if (result != -1) {
        cout << "Финальная сумма всех очков: " << result << endl;
    } else {
        cout << "Программа завершена с ошибкой." << endl;
    }

    return 0;
}
