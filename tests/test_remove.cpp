#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  cout << "=== Test de Remove en B-Tree ===" << endl << endl;
  
  BTree<int>* btree = new BTree<int>(3);
  
  // Insertar elementos
  int numbers[] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
  cout << "Insertando: ";
  for (int i = 0; i < 10; i++) {
    cout << numbers[i] << " ";
    btree->insert(numbers[i]);
  }
  cout << endl;
  
  cout << "Arbol despues de insertar: " << btree->toString(" ") << endl;
  cout << "Altura: " << btree->height() << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << "Min: " << btree->minKey() << ", Max: " << btree->maxKey() << endl;
  cout << endl;
  
  // Probar remove
  cout << "Eliminando 100..." << endl;
  btree->remove(100);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Eliminando 111..." << endl;
  btree->remove(111);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Eliminando 45..." << endl;
  btree->remove(45);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Altura: " << btree->height() << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Eliminando 114..." << endl;
  btree->remove(114);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Insertando 40..." << endl;
  btree->insert(40);
  cout << "Arbol: " << btree->toString(" - ") << endl;
  cout << "Altura: " << btree->height() << endl;
  cout << endl;
  
  cout << "Eliminando 11..." << endl;
  btree->remove(11);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << ", Max: " << btree->maxKey() << endl;
  cout << "Buscar 11: " << (btree->search(11) ? "Encontrado" : "No encontrado") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "=== Test completado exitosamente! ===" << endl;
  
  delete btree;
  return 0;
}
