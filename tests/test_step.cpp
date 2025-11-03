#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  BTree<int>* btree = new BTree<int>(3);
  
  // Solo un elemento
  btree->insert(100);
  cout << "Arbol con 1 elemento: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << ", Max: " << btree->maxKey() << endl;
  cout << endl;
  
  // Dos elementos
  btree->insert(50);
  cout << "Arbol con 2 elementos: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << ", Max: " << btree->maxKey() << endl;
  cout << endl;
  
  // Tres elementos (llena el nodo para M=3)
  btree->insert(150);
  cout << "Arbol con 3 elementos: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << ", Max: " << btree->maxKey() << endl;
  cout << endl;
  
  // Cuarto elemento (causa split)
  btree->insert(200);
  cout << "Arbol con 4 elementos (después de split): " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << ", Max: " << btree->maxKey() << endl;
  cout << "Height: " << btree->height() << endl;
  
  delete btree;
  return 0;
}
