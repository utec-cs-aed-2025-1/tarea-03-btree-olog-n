#include <iostream>
#include "btree.h"

using namespace std;

// Función para imprimir estructura del árbol
template<typename TK>
void printStructure(Node<TK>* node, int level = 0, string prefix = "") {
    if (node == nullptr) return;
    
    for (int i = 0; i < level; i++) cout << "  ";
    cout << prefix << "[";
    for (int i = 0; i < node->count; i++) {
        if (i > 0) cout << ",";
        cout << node->keys[i];
    }
    cout << "] (leaf=" << node->leaf << ")" << endl;
    
    if (!node->leaf) {
        for (int i = 0; i <= node->count; i++) {
            printStructure(node->children[i], level + 1, "C" + to_string(i) + ": ");
        }
    }
}

int main() {
    BTree<int>* btree = new BTree<int>(3);
    int numbers[10] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
    
    for (int i = 0; i < 10; i++) {
        btree->insert(numbers[i]);
    }
    
    cout << "=== Estructura del árbol después de 10 inserts ===" << endl;
    // No puedo acceder a root directamente...
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Height: " << btree->height() << endl;
    cout << endl;
    
    cout << "El test espera height=2 pero obtiene " << btree->height() << endl;
    cout << "El test espera: \"11 36 45 47 70 75 100 111 114 120\"" << endl;
    cout << "Pero obtiene:   \"" << btree->toString(" ") << "\"" << endl;
    
    delete btree;
    return 0;
}
