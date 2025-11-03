#include <iostream>
#include "btree.h"

using namespace std;

void printNodeInfo(const char* label) {
    cout << label << endl;
}

int main() {
  BTree<int>* btree = new BTree<int>(3);
  
  int numbers[] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
  for (int i = 0; i < 10; i++) {
    btree->insert(numbers[i]);
  }
  
  cout << "Arbol inicial: " << btree->toString(" ") << endl;
  cout << "Size: " << btree->size() << ", Height: " << btree->height() << endl;
  
  try {
    cout << "Min: " << btree->minKey() << endl;
  } catch(...) {
    cout << "Error en minKey" << endl;
  }
  
  try {
    cout << "Max: " << btree->maxKey() << endl;
  } catch(...) {
    cout << "Error en maxKey" << endl;
  }
  
  delete btree;
  return 0;
}
