#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Структура для множества
struct MySet {
    vector<string> elements;
};

// Функция проверки наличия элемента в множестве
bool contains(const MySet& set, const string& element) {
    for (const auto& elem : set.elements) {
        if (elem == element) {
            return true;
        }
    }
    return false;
}

// Функция добавления элемента в множество
void addElement(MySet& set, const string& element) {
    if (element.empty()) {
        cout << "Ошибка: нельзя добавить пустой элемент" << endl;
        return;
    }
    if (!contains(set, element)) {
        set.elements.push_back(element);
        cout << "Элемент '" << element << "' добавлен в множество" << endl;
    } else {
        cout << "Элемент '" << element << "' уже есть в множестве" << endl;
    }
}

// Функция удаления элемента из множества
void removeElement(MySet& set, const string& element) {
    for (auto it = set.elements.begin(); it != set.elements.end(); ++it) {
        if (*it == element) {
            set.elements.erase(it);
            cout << "Элемент '" << element << "' удален из множества" << endl;
            return;
        }
    }
    cout << "Элемент '" << element << "' не найден в множестве" << endl;
}

// Функция проверки наличия элемента в множестве
void checkElement(const MySet& set, const string& element) {
    if (contains(set, element)) {
        cout << "Элемент '" << element << "' присутствует в множестве" << endl;
    } else {
        cout << "Элемент '" << element << "' отсутствует в множестве" << endl;
    }
}

// Функция вывода множества
void printSet(const MySet& set, const string& name = "Множество") {
    cout << name << " (" << set.elements.size() << " элементов): ";
    if (set.elements.empty()) {
        cout << "пусто";
    } else {
        for (size_t i = 0; i < set.elements.size(); ++i) {
            cout << set.elements[i];
            if (i < set.elements.size() - 1) cout << ", ";
        }
    }
    cout << endl;
}

// Функция объединения множеств
MySet unionSets(const MySet& set1, const MySet& set2) {
    MySet result = set1;

    for (const auto& element : set2.elements) {
        if (!contains(result, element)) {
            result.elements.push_back(element);
        }
    }

    return result;
}

// Функция пересечения множеств
MySet intersectSets(const MySet& set1, const MySet& set2) {
    MySet result;

    for (const auto& element : set1.elements) {
        if (contains(set2, element)) {
            result.elements.push_back(element);
        }
    }

    return result;
}

// Функция разности множеств (set1 - set2)
MySet differenceSets(const MySet& set1, const MySet& set2) {
    MySet result;

    for (const auto& element : set1.elements) {
        if (!contains(set2, element)) {
            result.elements.push_back(element);
        }
    }

    return result;
}

// Функция создания множества из ввода пользователя
MySet createSetFromInput(const string& setName) {
    MySet set;
    cout << "Создание множества " << setName << ":" << endl;
    cout << "Введите элементы (пустая строка для завершения):" << endl;

    string element;
    while (true) {
        cout << "Элемент: ";
        getline(cin, element);

        if (element.empty()) {
            break;
        }

        addElement(set, element);
    }

    return set;
}

int main() {
    MySet setA, setB;
    int choice;

    cout << "=== Операции над множествами ===" << endl;

    while (true) {
        cout << endl << "Меню:" << endl;
        cout << "1. Создать/изменить множество A" << endl;
        cout << "2. Создать/изменить множество B" << endl;
        cout << "3. Показать множества" << endl;
        cout << "4. Объединение A ∪ B" << endl;
        cout << "5. Пересечение A ∩ B" << endl;
        cout << "6. Разность A \\ B" << endl;
        cout << "7. Разность B \\ A" << endl;
        cout << "8. Проверить элемент в множестве" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите операцию: ";

        cin >> choice;
        cin.ignore(); // Очистка буфера

        switch (choice) {
            case 1: {
                setA = createSetFromInput("A");
                break;
            }
            case 2: {
                setB = createSetFromInput("B");
                break;
            }
            case 3: {
                printSet(setA, "Множество A");
                printSet(setB, "Множество B");
                break;
            }
            case 4: {
                MySet unionResult = unionSets(setA, setB);
                cout << "Результат объединения A ∪ B:" << endl;
                printSet(unionResult, "A ∪ B");
                break;
            }
            case 5: {
                MySet intersectResult = intersectSets(setA, setB);
                cout << "Результат пересечения A ∩ B:" << endl;
                printSet(intersectResult, "A ∩ B");
                break;
            }
            case 6: {
                MySet diffResult = differenceSets(setA, setB);
                cout << "Результат разности A \\ B:" << endl;
                printSet(diffResult, "A \\ B");
                break;
            }
            case 7: {
                MySet diffResult = differenceSets(setB, setA);
                cout << "Результат разности B \\ A:" << endl;
                printSet(diffResult, "B \\ A");
                break;
            }
            case 8: {
                string element;
                int setChoice;

                cout << "Введите элемент для проверки: ";
                getline(cin, element);
                cout << "В каком множестве проверить? (1 - A, 2 - B): ";
                cin >> setChoice;
                cin.ignore();

                if (setChoice == 1) {
                    checkElement(setA, element);
                } else if (setChoice == 2) {
                    checkElement(setB, element);
                } else {
                    cout << "Неверный выбор множества" << endl;
                }
                break;
            }
            case 0: {
                cout << "Выход из программы" << endl;
                return 0;
            }
            default: {
                cout << "Неверный выбор операции" << endl;
                break;
            }
        }
    }

    return 0;
}
