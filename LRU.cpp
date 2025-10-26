#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Узел двусвязного списка
struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
};

// Структура LRU кэша
struct LRUCache {
    int capacity;
    int size;
    Node* head;
    Node* tail;
    Node** keyMap; // Массив для хранения указателей на узлы по ключам
    int maxKey;    // Максимальный возможный ключ
};

// Создание нового узла
Node* createNode(int key, int value) {
    Node* newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    return newNode;
}

// Инициализация кэша
LRUCache* initCache(int capacity) {
    if (capacity <= 0) {
        cerr << "Емкость кэша должна быть положительной" << endl;
        exit(1);
    }

    LRUCache* cache = new LRUCache();
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = createNode(-1, -1); // фиктивный головной узел
    cache->tail = createNode(-1, -1); // фиктивный хвостовой узел
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;
    
    // Предполагаем, что ключи не превышают 10000
    cache->maxKey = 10000;
    cache->keyMap = new Node*[cache->maxKey + 1];
    for (int i = 0; i <= cache->maxKey; i++) {
        cache->keyMap[i] = nullptr;
    }
    
    return cache;
}

// Удаление узла из списка
void removeNode(Node* node) {
    Node* prevNode = node->prev;
    Node* nextNode = node->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
}

// Добавление узла в начало списка (после головного)
void addToFront(LRUCache* cache, Node* node) {
    node->next = cache->head->next;
    node->prev = cache->head;
    cache->head->next->prev = node;
    cache->head->next = node;
}

// Перемещение узла в начало списка
void moveToFront(LRUCache* cache, Node* node) {
    removeNode(node);
    addToFront(cache, node);
}

// Удаление последнего узла (перед хвостовым)
void removeLast(LRUCache* cache) {
    if (cache->size == 0) return;

    Node* lastNode = cache->tail->prev;
    removeNode(lastNode);
    cache->keyMap[lastNode->key] = nullptr;
    delete lastNode;
    cache->size--;
}

// Установка значения
void setValue(LRUCache* cache, int key, int value) {
    if (key < 0 || key > cache->maxKey) {
        cerr << "Ключ вне допустимого диапазона" << endl;
        return;
    }
    
    if (cache->keyMap[key] != nullptr) {
        // Ключ уже существует, обновляем значение и перемещаем в начало
        Node* node = cache->keyMap[key];
        node->value = value;
        moveToFront(cache, node);
    } else {
        // Новый ключ
        if (cache->size == cache->capacity) {
            // Удаляем наименее используемый элемент
            removeLast(cache);
        }

        // Создаем новый узел и добавляем в начало
        Node* newNode = createNode(key, value);
        addToFront(cache, newNode);
        cache->keyMap[key] = newNode;
        cache->size++;
    }
}

// Получение значения
int getValue(LRUCache* cache, int key) {
    if (key < 0 || key > cache->maxKey) {
        return -1;
    }
    
    if (cache->keyMap[key] != nullptr) {
        Node* node = cache->keyMap[key];
        moveToFront(cache, node);
        return node->value;
    }
    return -1;
}

// Освобождение памяти
void freeCache(LRUCache* cache) {
    Node* current = cache->head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    delete[] cache->keyMap;
    delete cache;
}

int main() {
    int cap, Q;

    // Ввод емкости кэша и количества запросов
    cout << "Введите емкость кэша: ";
    if (!(cin >> cap) || cap <= 0) {
        cerr << "Некорректная емкость кэша" << endl;
        return 1;
    }

    cout << "Введите количество запросов: ";
    if (!(cin >> Q) || Q <= 0) {
        cerr << "Некорректное количество запросов" << endl;
        return 1;
    }

    LRUCache* cache = initCache(cap);
    vector<int> results; // Вектор для хранения результатов GET

    cout << "Введите запросы (SET x y или GET x):" << endl;

    for (int i = 0; i < Q; i++) {
        string operation;
        cin >> operation;

        if (operation == "SET") {
            int x, y;
            if (!(cin >> x >> y)) {
                cerr << "Некорректные параметры для SET" << endl;
                freeCache(cache);
                return 1;
            }
            setValue(cache, x, y);
        } else if (operation == "GET") {
            int x;
            if (!(cin >> x)) {
                cerr << "Некорректный параметр для GET" << endl;
                freeCache(cache);
                return 1;
            }
            int result = getValue(cache, x);
            results.push_back(result); // Сохраняем результат
        } else {
            cerr << "Неизвестная операция: " << operation << endl;
            freeCache(cache);
            return 1;
        }
    }

    // Вывод всех результатов GET в конце
    for (size_t i = 0; i < results.size(); i++) {
        cout << results[i];
        if (i != results.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;

    freeCache(cache);
    return 0;
}
