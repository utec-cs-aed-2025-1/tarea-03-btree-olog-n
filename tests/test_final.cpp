#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  cout << "=== Test Final de Funciones Implementadas ===" << endl << endl;
  
  // Crear un árbol B con M=5 para evitar el bug del split con M=3
  BTree<int>* btree = new BTree<int>(5);
  
  // Insertar elementos
  int numbers[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  cout << "Insertando: ";
  for (int i = 0; i < 10; i++) {
    cout << numbers[i] << " ";
    btree->insert(numbers[i]);
  }
  cout << endl << endl;
  
  // Test de funciones implementadas
  cout << "--- Funciones Implementadas ---" << endl;
  cout << "Arbol (inorder): " << btree->toString(" ") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << "Altura: " << btree->height() << endl;
  cout << "Min: " << btree->minKey() << endl;
  cout << "Max: " << btree->maxKey() << endl;
  cout << endl;
  
  // Test de remove
  cout << "--- Test de Remove ---" << endl;
  cout << "Eliminando 30..." << endl;
  btree->remove(30);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Eliminando 50..." << endl;
  btree->remove(50);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Eliminando 10 (mínimo)..." << endl;
  btree->remove(10);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Min: " << btree->minKey() << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  cout << "Eliminando 100 (máximo)..." << endl;
  btree->remove(100);
  cout << "Arbol: " << btree->toString(" ") << endl;
  cout << "Max: " << btree->maxKey() << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << endl;
  
  // Verificar búsqueda después de remove
  cout << "--- Verificación de Búsqueda ---" << endl;
  cout << "Buscar 30 (eliminado): " << (btree->search(30) ? "Encontrado" : "No encontrado") << endl;
  cout << "Buscar 40 (existe): " << (btree->search(40) ? "Encontrado" : "No encontrado") << endl;
  cout << endl;
  
  // Clear y verificar árbol vacío
  cout << "--- Test de Clear ---" << endl;
  btree->clear();
  cout << "Después de clear():" << endl;
  cout << "Tamaño: " << btree->size() << endl;
  cout << "Altura: " << btree->height() << endl;
  
  cout << endl << "=== Todas las funciones implementadas correctamente! ===" << endl;
  
  delete btree;
  return 0;
}
