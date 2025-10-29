#include <iostream>
#include <string>
using namespace std;

struct MyArray {
    char* data;
    int size;
    int capacity;
};

void initArray(MyArray& arr, int cap = 8) {
    arr.data = new char[cap];
    arr.size = 0;
    arr.capacity = cap;
}
//добавление элемента
void pushBackArray(MyArray& arr, char c) {
    if (arr.size >= arr.capacity) {
        int newCap = arr.capacity * 2;
        char* newData = new char[newCap];
        for (int i = 0; i < arr.size; ++i)
            newData[i] = arr.data[i];
        delete[] arr.data;
        arr.data = newData;
        arr.capacity = newCap;
    }
    arr.data[arr.size++] = c;
}
//освобождение памяти
void freeArray(MyArray& arr) {
    delete[] arr.data;
    arr.data = nullptr;
    arr.size = 0;
    arr.capacity = 0;
}

// Структура результата проверки
struct PatternMatchResult {
    bool isMatch;
    string message;
};

// Функция для проверки соответствия строки шаблону
PatternMatchResult matchPattern(const MyArray& str, const MyArray& pattern) {
    PatternMatchResult res;
    res.isMatch = false;
//позиции
    int strPos = 0;
    int patPos = 0;
    int starPos = -1;
    int matchPos = -1;

    while (strPos < str.size) {
//'?':
        if (patPos < pattern.size &&
            (pattern.data[patPos] == str.data[strPos] || pattern.data[patPos] == '?')) {
            strPos++;
            patPos++;
        }
//'*'
        else if (patPos < pattern.size && pattern.data[patPos] == '*') {
            starPos = patPos;
            matchPos = strPos;
            patPos++;
        }
//возврат к последней '*'
        else if (starPos != -1) {
            patPos = starPos + 1;
            matchPos++;
            strPos = matchPos;
        }

        else {
            res.message = "Строка не соответствует шаблону";
            return res;
        }
    }
//пропуск оставшихся '*'
    while (patPos < pattern.size && pattern.data[patPos] == '*')
        patPos++;

    if (patPos == pattern.size) {
        res.isMatch = true;
        res.message = "Строка соответствует шаблону";
    } else {
        res.message = "Строка не соответствует шаблону";
    }

    return res;
}

//Проверка корректности
bool isValid(const string& s, const string& name) {
    if (s.empty()) {
        cout << "Ошибка: " << name << " не может быть пустой\n";
        return false;
    }
    for (char c : s) {
        if (c < 32 || c > 126) {
            cout << "Ошибка: " << name << " содержит недопустимый символ\n";
            return false;
        }
    }
    return true;
}

// Основная программа
int main() {
    cout << "=== Проверка строки по шаблону (с самописным массивом) ===\n";
    cout << "Символы:\n"
         << "  ? — любой один символ\n"
         << "  * — любая последовательность символов\n";
    cout << "===========================================================\n";

    while (true) {
        cout << "\nМеню:\n";
        cout << "1. Проверить строку\n";
        cout << "0. Выход\n";
        cout << "Выбор: ";

        string choice;
        getline(cin, choice);

        if (choice == "0") {
            cout << "Выход из программы.\n";
            break;
        }
        else if (choice == "1") {
            string s, p;
            cout << "Введите строку: ";
            getline(cin, s);
            cout << "Введите шаблон: ";
            getline(cin, p);

            if (!isValid(s, "строка") || !isValid(p, "шаблон"))
                continue;

            // Переносим в MyArray
            MyArray str, pattern;
            initArray(str);
            initArray(pattern);
            for (char c : s) pushBackArray(str, c);
            for (char c : p) pushBackArray(pattern, c);

            // Проверка
            PatternMatchResult res = matchPattern(str, pattern);
            cout << "\nРезультат:\n";
            cout << "  Строка: \"" << s << "\"\n";
            cout << "  Шаблон: \"" << p << "\"\n";
            cout << "  " << res.message << endl;

            // Очистка
            freeArray(str);
            freeArray(pattern);
        }
        else {
            cout << "Неизвестная команда.\n";
        }
    }

    return 0;
}
