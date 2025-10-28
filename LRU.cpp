#include <iostream>
#include <string>

using namespace std;

//узел двусвязного списка
struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
};

//структура LRU кэша
struct LRUCache {
    int capacity;
    int size;//текущее кол-во элементов
    Node* head;
    Node* tail;
    Node** keyMap; //массив для хранения указателей на узлы по ключам
    int maxKey;    //максимальный возможный ключ
};

//создание нового узла
Node* createNode(int key, int value) {
    Node* newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    return newNode;
}

//инициализация кэша
LRUCache* initCache(int capacity) {
    if (capacity <= 0) {
        cerr << "Емкость кэша должна быть положительной" << endl;
        exit(1);
    }

    LRUCache* cache = new LRUCache();
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = createNode(-1, -1); //фиктивный головной узел
    cache->tail = createNode(-1, -1); //фиктивный хвостовой узел
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

    // Массив для хранения результатов GET
    int* results = new int[Q];
    int resultCount = 0; // Счетчик результатов

    cout << "Введите запросы (SET x y или GET x):" << endl;

    for (int i = 0; i < Q; i++) {
        string operation;
        cin >> operation;

        if (operation == "SET") {
            int x, y;
            if (!(cin >> x >> y)) {
                cerr << "Некорректные параметры для SET" << endl;
                freeCache(cache);
                delete[] results;
                return 1;
            }
            setValue(cache, x, y);
        } else if (operation == "GET") {
            int x;
            if (!(cin >> x)) {
                cerr << "Некорректный параметр для GET" << endl;
                freeCache(cache);
                delete[] results;
                return 1;
            }
            int result = getValue(cache, x);
            results[resultCount++] = result; // Сохраняем результат
        } else {
            cerr << "Неизвестная операция: " << operation << endl;
            freeCache(cache);
            delete[] results;
            return 1;
        }
    }

    // Вывод всех результатов GET в конце
    for (int i = 0; i < resultCount; i++) {
        cout << results[i];
        if (i != resultCount - 1) {
            cout << " ";
        }
    }
    cout << endl;

    freeCache(cache);
    delete[] results;
    return 0;
}
