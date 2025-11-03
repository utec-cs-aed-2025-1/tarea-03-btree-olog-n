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

    cout << "After 10 inserts:" << endl;
    cout << "  toString: " << btree->toString(" ") << endl;
    cout << "  height: " << btree->height() << " (should be 2)" << endl;

    btree->remove(116);
    cout << "\nAfter remove(116):" << endl;
    cout << "  toString: " << btree->toString(" ") << endl;
    cout << "  height: " << btree->height() << " (should be 2)" << endl;

    btree->remove(113);
    cout << "\nAfter remove(113):" << endl;
    cout << "  toString: " << btree->toString(" ") << endl;
    cout << "  height: " << btree->height() << " (should be 1)" << endl;

    btree->remove(118);
    cout << "\nAfter remove(118):" << endl;
    cout << "  toString: " << btree->toString(" ") << endl;
    cout << "  height: " << btree->height() << " (should be 1)" << endl;

    cout << "\nExpected at this point: '11 36 47 70 75 114 120' with height 1" << endl;
    
    btree->remove(114);
    btree->insert(40);
    
    cout << "\nAfter remove(114) and insert(40):" << endl;
    cout << "  toString: " << btree->toString(" - ") << endl;
    cout << "  height: " << btree->height() << " (should be 2)" << endl;
    cout << "  Expected: '11 - 36 - 40 - 47 - 70 - 75 - 120'" << endl;

    delete btree;
    return 0;
}
