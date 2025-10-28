#include <iostream>
#include <string>

using namespace std;

// Структура для узла дерева
struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
};

// Функция для создания нового узла
TreeNode* createNode(int value) {
    TreeNode* newNode = new TreeNode;
    newNode->value = value;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Функция для вставки значения в дерево
TreeNode* insert(TreeNode* root, int value) {
    if (root == nullptr) {
        return createNode(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else if (value > root->value) {
        root->right = insert(root->right, value);
    }

     return root;
}

// Функция для получения высоты узла
int getHeight(TreeNode* node) {
    if (node == nullptr) return 0;
    return max(getHeight(node->left), getHeight(node->right)) + 1;
}

// Функция для получения баланса узла
int getBalance(TreeNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Функция для проверки АВЛ-сбалансированности
bool isAVLBalanced(TreeNode* root) {
    if (root == nullptr) return true;

    int balance = getBalance(root);
    if (balance < -1 || balance > 1) {
        return false;
    }

    return isAVLBalanced(root->left) && isAVLBalanced(root->right);
}

// Функция для визуализации дерева с L, R и балансом
void printTree(TreeNode* root, string prefix = "", bool isLeft = true) {
    if (root == nullptr) return;

    cout << prefix;
    cout << (isLeft ? "├──L:" : "└──R:" );
    cout << root->value << "(b:" << getBalance(root) << ")" << endl;

    printTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
}

// Функция для освобождения памяти
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "=== Проверка АВЛ-сбалансированности дерева ===" << endl;

    // Ввод последовательности
    int sequence[100];
    int count = 0;
    int number;

    cout << "Введите последовательность целых чисел (0 для окончания ввода):" << endl;

    while (true) {
        cout << "Введите число: ";
        cin >> number;

        if (cin.fail()) {
            cout << "Ошибка: введите целое число!" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (number == 0) {
            break;
        }

        if (count < 100) {
            sequence[count] = number;
            count++;
        } else {
            cout << "Ошибка: превышен максимальный размер последовательности!" << endl;
            break;
        }
    }

    if (count == 0) {
        cout << "Ошибка: последовательность не может быть пустой!" << endl;
        return 1;
    }

    // Построение дерева
    TreeNode* root = nullptr;
    for (int i = 0; i < count; i++) {
        root = insert(root, sequence[i]);
    }

    // Вывод информации о дереве
    cout << "\n=== Построенное дерево ===" << endl;
    cout << "Последовательность: ";
    for (int i = 0; i < count; i++) {
        cout << sequence[i] << " ";
    }
    cout << endl;

    cout << "\nВизуализация дерева:" << endl;
    cout << "Корень:" << root->value << "(b:" << getBalance(root) << ")" << endl;
    printTree(root->left, "", true);
    printTree(root->right, "", false);

    // Проверка сбалансированности
    cout << "\n=== Результат проверки ===" << endl;
    if (isAVLBalanced(root)) {
        cout << "YES - дерево является АВЛ-сбалансированным" << endl;
    } else {
        cout << "NO - дерево не является АВЛ-сбалансированным" << endl;
    }

    // Освобождение памяти
    deleteTree(root);

   return 0;
}
