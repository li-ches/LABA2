#include <iostream>
#include <string>

using namespace std;

//Метод цепочек
struct HashNode {
    string key;
    string value;
    HashNode* next;
};

struct HashTableChaining {
    HashNode** table;
    int size;
};

//Открытая адресация
struct HashEntry {
    string key;
    string value;
    bool occupied;
};

struct HashTableOpenAddressing {
    HashEntry* table;
    int size;
};

// Создание таблиц
HashTableChaining createHashTableChaining(int size) {
    HashTableChaining ht;
    ht.size = size;
    ht.table = new HashNode*[size];
    for (int i = 0; i < size; i++) {
        ht.table[i] = nullptr;
    }
    return ht;
}

HashTableOpenAddressing createHashTableOpenAddressing(int size) {
    HashTableOpenAddressing ht;
    ht.size = size;
    ht.table = new HashEntry[size];
    for (int i = 0; i < size; i++) {
        ht.table[i].occupied = false;
    }
    return ht;
}

// Хеш-функция для строки
int hashFunction(const string& key, int tableSize) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + abs(c)) % tableSize;
    }
    return hash;
}

// Визуализация таблицы цепочек
void visualizeChaining(const HashTableChaining& ht) {
    cout << "ЦЕПОЧКИ: ";
    bool isEmpty = true;
    for (int i = 0; i < ht.size; i++) {
        if (ht.table[i] != nullptr) {
            isEmpty = false;
            cout << "[" << i << "]:";
            HashNode* current = ht.table[i];
            while (current != nullptr) {
                cout << current->key << "->" << current->value;
                if (current->next != nullptr) cout << "->";
                current = current->next;
            }
            cout << " ";
        }
    }
    if (isEmpty) {
        cout << "пусто";
    }
    cout << endl;
}

// Визуализация таблицы открытой адресации
void visualizeOpenAddressing(const HashTableOpenAddressing& ht) {
    cout << "ОТКР.АДР: ";
    bool isEmpty = true;
    for (int i = 0; i < ht.size; i++) {
        if (ht.table[i].occupied) {
            isEmpty = false;
            cout << "[" << i << "]" << ht.table[i].key << "->" << ht.table[i].value << " ";
        }
    }
    if (isEmpty) {
        cout << "пусто";
    }
    cout << endl;
}

// Метод цепочек: операции
void insertChaining(HashTableChaining& ht, const string& key, const string& value) {
    int index = hashFunction(key, ht.size);
    HashNode* newNode = new HashNode{key, value, nullptr};

    cout << "ЦЕПОЧКИ ВСТАВКА: " << key << "->" << value << " в ячейку " << index << endl;

    if (ht.table[index] == nullptr) {
        ht.table[index] = newNode;
        cout << "Создан новый узел" << endl;
    } else {
        HashNode* current = ht.table[index];
        while (current->next != nullptr) {
            if (current->key == key) {
                cout << "Ключ уже есть, обновлено: " << current->value << "->" << value << endl;
                current->value = value;
                delete newNode;
                visualizeChaining(ht);
                return;
            }
            current = current->next;
        }
        if (current->key == key) {
            cout << "Ключ уже есть, обновлено: " << current->value << "->" << value << endl;
            current->value = value;
            delete newNode;
        } else {
            current->next = newNode;
            cout << "Добавлен в конец цепочки" << endl;
        }
    }
    visualizeChaining(ht);
}

bool deleteChaining(HashTableChaining& ht, const string& key) {
    int index = hashFunction(key, ht.size);

    cout << "ЦЕПОЧКИ УДАЛЕНИЕ: " << key << " из ячейки " << index << endl;

    HashNode* current = ht.table[index];
    HashNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                ht.table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            cout << "Удален: " << key << "->" << current->value << endl;
            delete current;
            visualizeChaining(ht);
            return true;
        }
        prev = current;
        current = current->next;
    }
    cout << "Ключ не найден" << endl;
    return false;
}

string searchChaining(const HashTableChaining& ht, const string& key) {
    int index = hashFunction(key, ht.size);

    cout << "ЦЕПОЧКИ ПОИСК: " << key << " в ячейке " << index << endl;

    HashNode* current = ht.table[index];
    int steps = 0;

    while (current != nullptr) {
        steps++;
        if (current->key == key) {
            cout << "Найден за " << steps << " шагов: " << current->value << endl;
            return current->value;
        }
        current = current->next;
    }
    cout << "Не найден" << endl;
    return "";
}

// Открытая адресация: операции
void insertOpenAddressing(HashTableOpenAddressing& ht, const string& key, const string& value) {
    int index = hashFunction(key, ht.size);
    int originalIndex = index;
    int probes = 0;

    cout << "ОТКР.АДР ВСТАВКА: " << key << "->" << value << " начало в " << index << endl;

    do {
        probes++;
        cout << "Проба " << probes << ": ячейка " << index;

        if (!ht.table[index].occupied) {
            cout << " - свободна" << endl;
            ht.table[index].key = key;
            ht.table[index].value = value;
            ht.table[index].occupied = true;
            cout << "Размещено в ячейке " << index << endl;
            visualizeOpenAddressing(ht);
            return;
        }

        if (ht.table[index].key == key) {
            cout << " - ключ есть" << endl;
            cout << "Обновлено: " << ht.table[index].value << "->" << value << endl;
            ht.table[index].value = value;
            visualizeOpenAddressing(ht);
            return;
        }

        cout << " - занята" << endl;
        index = (index + 1) % ht.size;

    } while (index != originalIndex);

    cout << "Таблица переполнена!" << endl;
}

bool deleteOpenAddressing(HashTableOpenAddressing& ht, const string& key) {
    int index = hashFunction(key, ht.size);
    int originalIndex = index;
    int probes = 0;

    cout << "ОТКР.АДР УДАЛЕНИЕ: " << key << " начало в " << index << endl;

    do {
        probes++;
        cout << "Проба " << probes << ": ячейка " << index;

        if (ht.table[index].occupied && ht.table[index].key == key) {
            cout << " - найден" << endl;
            cout << "Удален: " << key << "->" << ht.table[index].value << endl;
            ht.table[index].occupied = false;
            visualizeOpenAddressing(ht);
            return true;
        }

        if (!ht.table[index].occupied) {
            cout << " - свободна" << endl;
            break;
        }

        cout << " - занята" << endl;
        index = (index + 1) % ht.size;

    } while (index != originalIndex);

    cout << "Ключ не найден" << endl;
    return false;
}

string searchOpenAddressing(const HashTableOpenAddressing& ht, const string& key) {
    int index = hashFunction(key, ht.size);
    int originalIndex = index;
    int probes = 0;

    cout << "ОТКР.АДР ПОИСК: " << key << " начало в " << index << endl;

    do {
        probes++;
        cout << "Проба " << probes << ": ячейка " << index;

        if (ht.table[index].occupied && ht.table[index].key == key) {
            cout << " - найден" << endl;
            cout << "Значение: " << ht.table[index].value << endl;
            return ht.table[index].value;
        }

        if (!ht.table[index].occupied) {
            cout << " - свободна" << endl;
            break;
        }

        cout << " - занята" << endl;
        index = (index + 1) % ht.size;

    } while (index != originalIndex);

    cout << "Не найден" << endl;
    return "";
}

// Визуализация изоморфности
bool areIsomorphicChaining(const string& str1, const string& str2) {
    if (str1.length() != str2.length()) {
        cout << "ОШИБКА: Строки разной длины!" << endl;
        cout << str1 << " (" << str1.length() << ") vs " << str2 << " (" << str2.length() << ")" << endl;
        return false;
    }

    cout << "ПРОВЕРКА ИЗОМОРФНОСТИ - ЦЕПОЧКИ" << endl;
    cout << "Строка 1: " << str1 << endl;
    cout << "Строка 2: " << str2 << endl;

    HashTableChaining mapping = createHashTableChaining(256);
    HashTableChaining reverseMapping = createHashTableChaining(256);

    for (size_t i = 0; i < str1.length(); i++) {
        string c1 = string(1, str1[i]);
        string c2 = string(1, str2[i]);

        cout << "Позиция " << i << ": '" << str1[i] << "' -> '" << str2[i] << "'" << endl;

        string existingMapping = searchChaining(mapping, c1);
        if (!existingMapping.empty()) {
            if (existingMapping != c2) {
                cout << "КОНФЛИКТ: '" << str1[i] << "' уже -> '" << existingMapping << "'" << endl;
                return false;
            }
        } else {
            insertChaining(mapping, c1, c2);
        }

        string existingReverseMapping = searchChaining(reverseMapping, c2);
        if (!existingReverseMapping.empty()) {
            if (existingReverseMapping != c1) {
                cout << "КОНФЛИКТ: '" << str2[i] << "' уже -> '" << existingReverseMapping << "'" << endl;
                return false;
            }
        } else {
            insertChaining(reverseMapping, c2, c1);
        }
    }

    cout << "ВСЕ ОТОБРАЖЕНИЯ КОРРЕКТНЫ - Строки изоморфны" << endl;
    return true;
}

// Визуализация операций
void manualHashTableOperations() {
    cout << "УПРАВЛЕНИЕ ХЕШ-ТАБЛИЦАМИ (размер: 10)" << endl;

    HashTableChaining htChain = createHashTableChaining(10);
    HashTableOpenAddressing htOpen = createHashTableOpenAddressing(10);

    while (true) {
        cout << endl;
        cout << "Команды: INSERT ключ значение, DELETE ключ, SEARCH ключ, BACK" << endl;
        cout << "Введите команду: ";

        string operation;
        getline(cin, operation);

        if (operation == "BACK") break;

        if (operation.find("INSERT") == 0) {
            string key, value;
            size_t pos = operation.find(' ');
            if (pos != string::npos) {
                size_t pos2 = operation.find(' ', pos + 1);
                if (pos2 != string::npos) {
                    key = operation.substr(pos + 1, pos2 - pos - 1);
                    value = operation.substr(pos2 + 1);
                    cout << endl;
                    insertChaining(htChain, key, value);
                    cout << endl;
                    insertOpenAddressing(htOpen, key, value);
                } else {
                    cout << "Ошибка формата! Используйте: INSERT ключ значение" << endl;
                }
            } else {
                cout << "Ошибка формата! Используйте: INSERT ключ значение" << endl;
            }
        }
        else if (operation.find("DELETE") == 0) {
            string key = operation.substr(7);
            if (!key.empty()) {
                cout << endl;
                deleteChaining(htChain, key);
                cout << endl;
                deleteOpenAddressing(htOpen, key);
            } else {
                cout << "Ошибка формата! Используйте: DELETE ключ" << endl;
            }
        }
        else if (operation.find("SEARCH") == 0) {
            string key = operation.substr(7);
            if (!key.empty()) {
                cout << endl;
                string result1 = searchChaining(htChain, key);
                cout << endl;
                string result2 = searchOpenAddressing(htOpen, key);

                cout << "ИТОГ ПОИСКА " << key << ":" << endl;
                cout << "Цепочки: ";
                if (result1.empty()) cout << "не найден"; else cout << result1;
                cout << endl;
                cout << "Открытая адресация: ";
                if (result2.empty()) cout << "не найден"; else cout << result2;
                cout << endl;
            } else {
                cout << "Ошибка формата! Используйте: SEARCH ключ" << endl;
            }
        }
        else {
            cout << "Неизвестная команда!" << endl;
        }
    }

    for (int i = 0; i < htChain.size; i++) {
        HashNode* current = htChain.table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] htChain.table;
    delete[] htOpen.table;
}

int main() {
    while (true) {
        cout << endl;
        cout << "МЕНЮ" << endl;
        cout << "1. Проверить изоморфность строк" << endl;
        cout << "2. Управление хеш-таблицами" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите: ";

        string choice;
        getline(cin, choice);

        if (choice == "0") {
            cout << "Выход из программы" << endl;
            break;
        }
        else if (choice == "1") {
            string str1, str2;

            cout << "Первая строка: ";
            getline(cin, str1);

            cout << "Вторая строка: ";
            getline(cin, str2);

            if (str1.empty() || str2.empty()) {
                cout << "Строки не могут быть пустыми!" << endl;
                continue;
            }

            bool resultChaining = areIsomorphicChaining(str1, str2);
            bool resultOpenAddressing = areIsomorphicChaining(str1, str2);

            cout << endl << "ФИНАЛЬНЫЕ РЕЗУЛЬТАТЫ:" << endl;
            cout << "Строка 1: " << str1 << endl;
            cout << "Строка 2: " << str2 << endl;
            cout << "Цепочки: " << (resultChaining ? "ИЗОМОРФНЫ" : "НЕ ИЗОМОРФНЫ") << endl;
            cout << "Открытая адресация: " << (resultOpenAddressing ? "ИЗОМОРФНЫ" : "НЕ ИЗОМОРФНЫ") << endl;
            if (resultChaining == resultOpenAddressing) {
                cout << "Оба метода согласны" << endl;
            } else {
                cout << "Методы расходятся" << endl;
            }
        }
        else if (choice == "2") {
            manualHashTableOperations();
        }
        else {
            cout << "Неизвестный выбор!" << endl;
        }
    }

    return 0;
}
