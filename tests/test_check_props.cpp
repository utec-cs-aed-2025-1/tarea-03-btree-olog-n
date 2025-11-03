#include <iostream>
#include "btree.h"

using namespace std;

int main() {
    BTree<int>* btree = new BTree<int>(3);
    int numbers[10] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
    
    for (int i = 0; i < 10; i++) {
        btree->insert(numbers[i]);
    }
    
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Size: " << btree->size() << endl;
    cout << "Height: " << btree->height() << endl;
    
    cout << "\nVerificando propiedades del B-Tree:" << endl;
    if (btree->check_properties()) {
        cout << "✓ El árbol cumple con todas las propiedades del B-Tree" << endl;
    } else {
        cout << "✗ El árbol NO cumple con las propiedades del B-Tree" << endl;
    }
    
    delete btree;
    return 0;
}
