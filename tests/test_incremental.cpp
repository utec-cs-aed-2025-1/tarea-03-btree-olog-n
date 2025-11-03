#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  BTree<int>* btree = new BTree<int>(3);
  
  int numbers[] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
  for (int i = 0; i < 10; i++) {
    btree->insert(numbers[i]);
    cout << "Después de insertar " << numbers[i] << ": " << btree->toString(" ") << endl;
    try {
      cout << "Max: " << btree->maxKey() << endl;
    } catch(...) {
      cout << "Error en maxKey!" << endl;
    }
    cout << endl;
  }
  
  delete btree;
  return 0;
}
