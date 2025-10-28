#include <iostream>
#include <string>
#include <cctype>
#include <stack>

using namespace std;

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
int processOperations(const string* operations, int operationsCount) {
    stack<int> scores;//наш стек

    for (int i = 0; i < operationsCount; i++) {
        string op = operations[i];

        if (isNumber(op)) {
            // Преобразуем строку в число и добавляем в стек
            int score = stoi(op);
            if (score <= 0) {
                cout << "Ошибка: счет должен быть положительным числом. Получено: " << op << endl;
                return -1;
            }
            scores.push(score);
            cout << "Добавлен счет: " << score << endl;
        }
        else if (op == "+") {
            // Сумма двух предыдущих очков
            if (scores.size() < 2) {
                cout << "Ошибка: операция '+' требует как минимум два предыдущих счета" << endl;
                return -1;
            }
            int last = scores.top();
            scores.pop();
            int secondLast = scores.top();
            scores.pop();
            int newScore = last + secondLast;
            scores.push(secondLast);
            scores.push(last);
            scores.push(newScore);
            cout << "Добавлена сумма: " << last << " + " << secondLast << " = " << newScore << endl;
        }
        else if (op == "D") {
            // Удвоение последнего счета
            if (scores.size() < 1) {
                cout << "Ошибка: операция 'D' требует как минимум один предыдущий счет" << endl;
                return -1;
            }
            int last = scores.top();
            int newScore = last * 2;
            scores.push(newScore);
            cout << "Удвоен счет: " << last << " * 2 = " << newScore << endl;
        }
        else if (op == "C") {
            // Удаление последнего счета
            if (scores.size() < 1) {
                cout << "Ошибка: операция 'C' требует как минимум один предыдущий счет" << endl;
                return -1;
            }
            int removed = scores.top();
            scores.pop();
            cout << "Удален счет: " << removed << endl;
        }
        else {
            cout << "Ошибка: неизвестная операция '" << op << "'" << endl;
            return -1;
        }

        // Вывод текущего состояния
        cout << "Текущие счета: ";
        if (scores.empty()) {
            cout << "пусто";
        } else {
            // Для вывода создаем временную копию и выводим в правильном порядке
            stack<int> temp = scores;
            stack<int> reverse;

            // Перекладываем в обратный стек
            while (!temp.empty()) {
                reverse.push(temp.top());
                temp.pop();
            }

            // Выводим из обратного стека
            while (!reverse.empty()) {
                cout << reverse.top();
                reverse.pop();
                if (!reverse.empty()) cout << ", ";
            }
        }
        cout << endl << endl;
    }

    // Вычисляем сумму всех очков
    int total = 0;
    while (!scores.empty()) {
        total += scores.top();
        scores.pop();
    }

    return total;
}

// Функция для ввода операций
string* inputOperations(int& operationsCount) {
    string* operations = nullptr;
    string input;

    cout << "Введите количество операций: ";
    cin >> operationsCount;

    if (operationsCount <= 0) {
        cout << "Количество операций должно быть положительным числом!" << endl;
        operationsCount = 0;
        return operations;
    }

    operations = new string[operationsCount];

    cout << "Введите " << operationsCount << " операций:" << endl;
    cout << "(число - добавить счет, '+' - сумма двух предыдущих, 'D' - удвоить, 'C' - удалить)" << endl;

    for (int i = 0; i < operationsCount; i++) {
        cout << "Операция " << (i + 1) << ": ";
        cin >> input;
        operations[i] = input;
    }

    return operations;
}

int main() {
    cout << "=== Учет очков в бейсбольной игре ===" << endl << endl;

    // Ввод операций от пользователя
    int operationsCount = 0;
    string* operations = inputOperations(operationsCount);

    if (operationsCount == 0 || operations == nullptr) {
        cout << "Нет операций для обработки." << endl;
        return 0;
    }

    cout << endl << "Обрабатываем операции..." << endl;
    cout << "=========================================" << endl;

    // Обработка операций
    int result = processOperations(operations, operationsCount);

    // Освобождаем память
    delete[] operations;

    cout << "=========================================" << endl;

    if (result != -1) {
        cout << "Финальная сумма всех очков: " << result << endl;
    } else {
        cout << "Программа завершена с ошибкой." << endl;
    }

    return 0;
}
