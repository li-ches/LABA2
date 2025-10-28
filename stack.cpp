#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Хранение стека очков
struct BaseballGame {
    int* scores;
    int capacity;
    int size;
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

// Функция для инициализации игры
void initializeGame(BaseballGame& game, int initialCapacity = 10) {
    game.scores = new int[initialCapacity];
    game.capacity = initialCapacity;
    game.size = 0;
}

// Функция для добавления счета в массив
void pushScore(BaseballGame& game, int score) {
    if (game.size >= game.capacity) {
        // Увеличиваем емкость массива
        int newCapacity = game.capacity * 2;
        int* newScores = new int[newCapacity];

        // Копируем существующие элементы
        for (int i = 0; i < game.size; i++) {
            newScores[i] = game.scores[i];
        }

        // Освобождаем старый массив и обновляем указатель
        delete[] game.scores;
        game.scores = newScores;
        game.capacity = newCapacity;
    }

    game.scores[game.size] = score;
    game.size++;
}

// Функция для удаления последнего счета
void popScore(BaseballGame& game) {
    if (game.size > 0) {
        game.size--;
    }
}

// Функция для получения последнего счета
int getLastScore(BaseballGame& game) {
    if (game.size > 0) {
        return game.scores[game.size - 1];
    }
    return -1;
}

// Функция для получения предпоследнего счета
int getSecondLastScore(BaseballGame& game) {
    if (game.size > 1) {
        return game.scores[game.size - 2];
    }
    return -1;
}

// Функция для освобождения памяти
void cleanupGame(BaseballGame& game) {
    delete[] game.scores;
    game.scores = nullptr;
    game.capacity = 0;
    game.size = 0;
}

// Функция для обработки операций
int processOperations(const string* operations, int operationsCount) {
    BaseballGame game;
    initializeGame(game);

    for (int i = 0; i < operationsCount; i++) {
        string op = operations[i];

        if (isNumber(op)) {
            // Преобразуем строку в число и добавляем в стек
            int score = stoi(op);
            if (score <= 0) {
                cout << "Ошибка: счет должен быть положительным числом. Получено: " << op << endl;
                cleanupGame(game);
                return -1;
            }
            pushScore(game, score);
            cout << "Добавлен счет: " << score << endl;
        }
        else if (op == "+") {
            // Сумма двух предыдущих очков
            if (game.size < 2) {
                cout << "Ошибка: операция '+' требует как минимум два предыдущих счета" << endl;
                cleanupGame(game);
                return -1;
            }
            int last = getLastScore(game);
            int secondLast = getSecondLastScore(game);
            int newScore = last + secondLast;
            pushScore(game, newScore);
            cout << "Добавлена сумма: " << last << " + " << secondLast << " = " << newScore << endl;
        }
        else if (op == "D") {
            // Удвоение последнего счета
            if (game.size < 1) {
                cout << "Ошибка: операция 'D' требует как минимум один предыдущий счет" << endl;
                cleanupGame(game);
                return -1;
            }
            int last = getLastScore(game);
            int newScore = last * 2;
            pushScore(game, newScore);
            cout << "Удвоен счет: " << last << " * 2 = " << newScore << endl;
        }
        else if (op == "C") {
            // Удаление последнего счета
            if (game.size < 1) {
                cout << "Ошибка: операция 'C' требует как минимум один предыдущий счет" << endl;
                cleanupGame(game);
                return -1;
            }
            int removed = getLastScore(game);
            popScore(game);
            cout << "Удален счет: " << removed << endl;
        }
        else {
            cout << "Ошибка: неизвестная операция '" << op << "'" << endl;
            cleanupGame(game);
            return -1;
        }

        // Вывод текущего состояния
        cout << "Текущие счета: ";
        if (game.size == 0) {
            cout << "пусто";
        } else {
            for (int j = 0; j < game.size; j++) {
                cout << game.scores[j];
                if (j < game.size - 1) cout << ", ";
            }
        }
        cout << endl << endl;
    }

    // Вычисляем сумму всех очков
    int total = 0;
    for (int i = 0; i < game.size; i++) {
        total += game.scores[i];
    }

    cleanupGame(game);
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
