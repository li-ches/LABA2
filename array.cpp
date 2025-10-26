#include <iostream>
#include <string>

using namespace std;

// Структура для хранения результата проверки
struct PatternMatchResult {
    bool isMatch;
    string message;
};

// Функция для проверки соответствия строки шаблону
PatternMatchResult matchPattern(const string& str, const string& pattern) {
    PatternMatchResult result;
    result.isMatch = false;

    int strPos = 0;
    int patternPos = 0;
    int starPos = -1;
    int matchPos = -1;

    while (strPos < str.length()) {
        // Если символы совпадают или в шаблоне '?'
        if (patternPos < pattern.length() &&
            (pattern[patternPos] == str[strPos] || pattern[patternPos] == '?')) {
            strPos++;
            patternPos++;
        }
        // Если встречаем '*'
        else if (patternPos < pattern.length() && pattern[patternPos] == '*') {
            starPos = patternPos;
            matchPos = strPos;
            patternPos++;
        }
        // Если был '*' и не совпало - возвращаемся
        else if (starPos != -1) {
            patternPos = starPos + 1;
            matchPos++;
            strPos = matchPos;
        }
        // Если не совпало и не было '*'
        else {
            result.message = "Строка не соответствует шаблону";
            return result;
        }
    }

    // Пропускаем оставшиеся '*' в шаблоне
    while (patternPos < pattern.length() && pattern[patternPos] == '*') {
        patternPos++;
    }

    // Если дошли до конца шаблона - совпадение
    if (patternPos == pattern.length()) {
        result.isMatch = true;
        result.message = "Строка соответствует шаблону";
    } else {
        result.message = "Строка не соответствует шаблону";
    }

    return result;
}

// Функция проверки корректности шаблона
bool isValidPattern(const string& pattern) {
    if (pattern.empty()) {
        cout << "Ошибка: шаблон не может быть пустым" << endl;
        return false;
    }

    for (char c : pattern) {
        if (c < 32 || c > 126) {
            cout << "Ошибка: шаблон содержит недопустимый символ" << endl;
            return false;
        }
    }

    return true;
}

// Функция проверки корректности строки
bool isValidString(const string& str) {
    if (str.empty()) {
        cout << "Ошибка: строка не может быть пустой" << endl;
        return false;
    }

    for (char c : str) {
        if (c < 32 || c > 126) {
            cout << "Ошибка: строка содержит недопустимый символ" << endl;
            return false;
        }
    }

    return true;
}

int main() {
    cout << "=== Проверка соответствия строки шаблону ===" << endl;
    cout << "Специальные символы:" << endl;
    cout << "  ? - один любой символ" << endl;
    cout << "  * - любая последовательность символов" << endl;
    cout << "=============================================" << endl;
    
    while (true) {
        cout << "\nМеню:" << endl;
        cout << "1. Проверить соответствие строки шаблону" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите операцию: ";

        string choice;
        getline(cin, choice);

        if (choice == "0") {
            cout << "Выход из программы." << endl;
            break;
        }
        else if (choice == "1") {
            string str, pattern;
            cout << "Введите строку: ";
            getline(cin, str);

            cout << "Введите шаблон: ";
            getline(cin, pattern);

            // Проверка корректности ввода
            if (!isValidString(str) || !isValidPattern(pattern)) {
                cout << "Попробуйте снова." << endl;
                continue;
            }

            // Проверка соответствия
            PatternMatchResult result = matchPattern(str, pattern);

            cout << "\nРезультат проверки:" << endl;
            cout << "Строка: \"" << str << "\"" << endl;
            cout << "Шаблон: \"" << pattern << "\"" << endl;
            cout << "Результат: " << result.message << endl;
        }
        else {
            cout << "Неизвестная операция. Попробуйте снова." << endl;
        }
    }

    return 0;
}
