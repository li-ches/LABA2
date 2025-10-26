#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для элемента множества
struct SetElement {
    string value;
    SetElement* next;
};

// Структура для множества
struct MySet {
    SetElement* first;
    string filename;
};

// Функция для создания нового элемента
SetElement* createElement(const string& value) {
    SetElement* newElement = new SetElement;
    newElement->value = value;
    newElement->next = nullptr;
    return newElement;
}

// Функция проверки наличия элемента в множестве
bool contains(const MySet& set, const string& element) {
    SetElement* current = set.first;
    while (current != nullptr) {
        if (current->value == element) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Функция добавления элемента в множество
void SETADD(MySet& set, const string& element) {
    if (element.empty()) {
        cout << "Ошибка: нельзя добавить пустой элемент" << endl;
        return;
    }

    if (contains(set, element)) {
        cout << "Элемент '" << element << "' уже существует в множестве" << endl;
        return;
    }

    SetElement* newElement = createElement(element);
    newElement->next = set.first;
    set.first = newElement;
    cout << "Элемент '" << element << "' добавлен в множество" << endl;
}

// Функция удаления элемента из множества
void SETDEL(MySet& set, const string& element) {
    if (set.first == nullptr) {
        cout << "Элемент '" << element << "' не найден в множестве" << endl;
        return;
    }

    // Если элемент первый в списке
    if (set.first->value == element) {
        SetElement* toDelete = set.first;
        set.first = set.first->next;
        delete toDelete;
        cout << "Элемент '" << element << "' удален из множества" << endl;
        return;
    }

    // Поиск элемента в списке
    SetElement* current = set.first;
    while (current->next != nullptr) {
        if (current->next->value == element) {
            SetElement* toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
            cout << "Элемент '" << element << "' удален из множества" << endl;
            return;
        }
        current = current->next;
    }

    cout << "Элемент '" << element << "' не найден в множестве" << endl;
}

// Функция проверки наличия элемента в множестве
void SET_AT(const MySet& set, const string& element) {
    if (contains(set, element)) {
        cout << "Элемент '" << element << "' присутствует в множестве" << endl;
    } else {
        cout << "Элемент '" << element << "' отсутствует в множестве" << endl;
    }
}

// Функция сохранения множества в файл
void saveToFile(const MySet& set) {
    if (set.filename.empty()) return;

    ofstream file(set.filename);
    if (file.is_open()) {
        SetElement* current = set.first;
        while (current != nullptr) {
            file << current->value << endl;
            current = current->next;
        }
        file.close();
    }
}

// Функция загрузки данных из файла
void loadFromFile(MySet& set, const string& filename) {
    set.filename = filename;
    set.first = nullptr;

    ifstream file(filename);

    // Если файл не существует, создаем пустой
    if (!file.is_open()) {
        ofstream createFile(filename);
        if (createFile.is_open()) {
            cout << "Создан новый файл '" << filename << "'" << endl;
            createFile.close();
        }
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty() && !contains(set, line)) {
            SETADD(set, line);
            count++;
        }
    }
    file.close();

    if (count > 0) {
        cout << "Загружено " << count << " элемент(ов) из файла '" << filename << "'" << endl;
    }
}

// Функция для освобождения памяти
void clearSet(MySet& set) {
    SetElement* current = set.first;
    while (current != nullptr) {
        SetElement* next = current->next;
        delete current;
        current = next;
    }
    set.first = nullptr;
}

// Функция обработки запроса
void processQuery(MySet& set, const string& query) {
    string operation, element;
    size_t space_pos = query.find(' ');

    if (space_pos != string::npos) {
        operation = query.substr(0, space_pos);
        element = query.substr(space_pos + 1);
    } else {
        operation = query;
    }

    if (operation == "SETADD" && !element.empty()) {
        SETADD(set, element);
        saveToFile(set);
    } else if (operation == "SETDEL" && !element.empty()) {
        SETDEL(set, element);
        saveToFile(set);
    } else if (operation == "SET_AT" && !element.empty()) {
        SET_AT(set, element);
    } else if (operation == "SHOW") {
        cout << "Текущее множество (" << [](const MySet& set) {
            int count = 0;
            SetElement* current = set.first;
            while (current != nullptr) {
                count++;
                current = current->next;
            }
            return count;
        }(set) << " элементов): ";

        if (set.first == nullptr) {
            cout << "пусто";
        } else {
            SetElement* current = set.first;
            while (current != nullptr) {
                cout << current->value << " ";
                current = current->next;
            }
        }
        cout << endl;
    } else {
        cout << "Ошибка: неизвестная операция '" << query << "'" << endl;
        cout << "Доступные операции: SETADD <элемент>, SETDEL <элемент>, SET_AT <элемент>, SHOW" << endl;
    }
}

int main(int argc, char* argv[]) {
    MySet mySet;
    mySet.first = nullptr;
    string filename;
    string query;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    // Если указан файл - загружаем данные
    if (!filename.empty()) {
        loadFromFile(mySet, filename);
    }

    // Если указан запрос - выполняем его
    if (!query.empty()) {
        processQuery(mySet, query);
    } else {
        // Интерактивный режим
        cout << "=== АТД Множество ===" << endl;
        cout << "Доступные операции:" << endl;
        cout << "SETADD <элемент> - добавить элемент" << endl;
        cout << "SETDEL <элемент> - удалить элемент" << endl;
        cout << "SET_AT <элемент> - проверить наличие элемента" << endl;
        cout << "SHOW - показать все элементы" << endl;
        cout << "EXIT - выход" << endl;
        cout << "=====================" << endl;

        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);

            if (input == "EXIT" || input == "exit") {
                break;
            }

            if (!input.empty()) {
                processQuery(mySet, input);
            }
        }
    }

    // Освобождение памяти
    clearSet(mySet);

    return 0;
}
