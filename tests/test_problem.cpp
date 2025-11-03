#include <iostream>
#include "btree.h"

using namespace std;

void printTreeStructure(const char* msg, BTree<int>* bt) {
    cout << msg << endl;
    cout << "  String: " << bt->toString(" ") << endl;
    cout << "  Height: " << bt->height() << endl;
    cout << "  Size: " << bt->size() << endl;
    try {
        cout << "  Min: " << bt->minKey() << ", Max: " << bt->maxKey() << endl;
    } catch (...) {
        cout << "  ERROR: No se pudo obtener min/max" << endl;
    }
    cout << endl;
}

int main() {
  BTree<int>* btree = new BTree<int>(3);
  
  btree->insert(45);
  btree->insert(75);
  btree->insert(100);
  btree->insert(36);
  btree->insert(120);
  btree->insert(70);
  btree->insert(11);
  
  printTreeStructure("Antes de insertar 111:", btree);
  
  btree->insert(111);
  
  printTreeStructure("Después de insertar 111:", btree);
  
  delete btree;
  return 0;
}
