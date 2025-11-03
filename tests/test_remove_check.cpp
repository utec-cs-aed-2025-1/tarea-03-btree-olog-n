#include <iostream>
#include "btree.h"

using namespace std;

int main() {
    BTree<int>* btree = new BTree<int>(3);
    int numbers[10] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
    
    for (int i = 0; i < 10; i++) {
        btree->insert(numbers[i]);
    }
    
    cout << "=== Después de inserts ===" << endl;
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Size: " << btree->size() << ", Height: " << btree->height() << endl;
    cout << "Check: " << (btree->check_properties() ? "✓" : "✗") << endl << endl;
    
    btree->remove(100);
    cout << "=== Después de remove(100) ===" << endl;
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Size: " << btree->size() << ", Height: " << btree->height() << endl;
    cout << "Check: " << (btree->check_properties() ? "✓" : "✗") << endl << endl;
    
    btree->remove(111);
    cout << "=== Después de remove(111) ===" << endl;
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Size: " << btree->size() << ", Height: " << btree->height() << endl;
    cout << "Check: " << (btree->check_properties() ? "✓" : "✗") << endl << endl;
    
    btree->remove(45);
    cout << "=== Después de remove(45) ===" << endl;
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Size: " << btree->size() << ", Height: " << btree->height() << endl;
    cout << "Check: " << (btree->check_properties() ? "✓" : "✗") << endl << endl;
    
    delete btree;
    return 0;
}
