#include "btree.h"
#include <iostream>
using namespace std;

void printTreeStructure(BTree<int>* btree) {
    // Crear una función para imprimir la estructura del árbol
    cout << "Tree structure (visualization):" << endl;
    cout << "Root: " << btree->toString(" ") << endl;
}

int main() {
    BTree<int>* btree = new BTree<int>(3);

    btree->insert(11);
    btree->insert(114);
    btree->insert(70);
    btree->insert(120);
    btree->insert(75);
    btree->insert(36);
    btree->insert(116);
    btree->insert(47);
    btree->insert(113);
    btree->insert(118);

    btree->remove(116);
    btree->remove(113);
    btree->remove(118);
    
    cout << "Before remove(114) and insert(40):" << endl;
    cout << "  Elements: " << btree->toString(" ") << endl;
    cout << "  Height: " << btree->height() << endl;
    cout << "  Count: 7 elements" << endl;
    
    btree->remove(114);
    cout << "\nAfter remove(114):" << endl;
    cout << "  Elements: " << btree->toString(" ") << endl;
    cout << "  Height: " << btree->height() << endl;
    cout << "  Count: 6 elements" << endl;
    
    btree->insert(40);
    cout << "\nAfter insert(40):" << endl;
    cout << "  Elements: " << btree->toString(" ") << endl;
    cout << "  Height: " << btree->height() << endl;
    cout << "  Count: 7 elements" << endl;
    
    cout << "\n¿Debe ser altura 1 o 2?" << endl;
    cout << "Para M=3, altura 1 = nodo raíz + hojas" << endl;
    cout << "7 elementos distribuidos en:" << endl;
    cout << "  - Si altura 1: raíz puede tener 1-2 keys, cada hoja 1-2 keys" << endl;
    cout << "  - Si altura 2: raíz + nodos internos + hojas" << endl;

    delete btree;
    return 0;
}
