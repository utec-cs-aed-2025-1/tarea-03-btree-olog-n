#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  BTree<int>* btree = new BTree<int>(3);
  
  int numbers[] = {45, 75, 100, 36, 120, 70, 11, 111};
  for (int i = 0; i < 8; i++) {
    btree->insert(numbers[i]);
  }
  
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Height: " << btree->height() << endl;
  cout << "Size: " << btree->size() << endl;
  cout << "Min: " << btree->minKey() << endl;
  
  cout << "\n=== Estructura del Árbol ===" << endl;
  btree->debugPrintStructure();
  
  // Intentar maxKey
  try {
    int max = btree->maxKey();
    cout << "\nMax: " << max << endl;
  } catch (exception& e) {
    cout << "\nException: " << e.what() << endl;
  } catch (...) {
    cout << "\nUnknown exception" << endl;
  }
  
  delete btree;
  return 0;
}
