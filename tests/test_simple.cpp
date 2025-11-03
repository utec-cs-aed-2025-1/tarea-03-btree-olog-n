#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  BTree<int>* btree = new BTree<int>(3);
  
  // Insertar pocos elementos
  btree->insert(10);
  btree->insert(20);
  btree->insert(30);
  
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << endl;
  cout << "Max: " << btree->maxKey() << endl;
  cout << "Height: " << btree->height() << endl;
  cout << "Size: " << btree->size() << endl;
  cout << endl;
  
  btree->insert(5);
  btree->insert(15);
  btree->insert(25);
  
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << endl;
  cout << "Max: " << btree->maxKey() << endl;
  cout << "Height: " << btree->height() << endl;
  cout << "Size: " << btree->size() << endl;
  
  delete btree;
  return 0;
}
