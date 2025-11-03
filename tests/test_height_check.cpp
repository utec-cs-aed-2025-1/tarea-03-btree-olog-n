#include "btree.h"
#include <iostream>
using namespace std;

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

    cout << "Arbol con 10 elementos:" << endl;
    cout << "toString: " << btree->toString(" - ") << endl;
    cout << "height: " << btree->height() << endl;
    
    btree->remove(114);
    btree->insert(40);
    
    cout << "\nDespues de remove(114) e insert(40):" << endl;
    cout << "toString: " << btree->toString(" - ") << endl;
    cout << "height: " << btree->height() << endl;
    cout << "Expected height: 2" << endl;

    delete btree;
    return 0;
}
