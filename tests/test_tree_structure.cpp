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

    btree->remove(116);
    btree->remove(113);
    btree->remove(118);
    btree->remove(114);
    
    cout << "After removals (before insert(40)):" << endl;
    cout << "Elements: " << btree->toString(" ") << endl;
    cout << "Height: " << btree->height() << endl;
    
    btree->insert(40);
    
    cout << "\nAfter insert(40):" << endl;
    cout << "Elements: " << btree->toString(" ") << endl;
    cout << "Height: " << btree->height() << endl;

    delete btree;
    return 0;
}
