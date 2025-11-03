#include <iostream>
#include "btree.h"

using namespace std;

// Función para imprimir la estructura interna de un nodo
template <typename TK>
void printNode(Node<TK>* node, int depth = 0) {
    if (node == nullptr) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "nullptr" << endl;
        return;
    }
    
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "Node[" << node << "] leaf=" << node->leaf << " count=" << node->count << " keys=[";
    for (int i = 0; i < node->count; i++) {
        if (i > 0) cout << ",";
        cout << node->keys[i];
    }
    cout << "]" << endl;
    
    if (!node->leaf) {
        for (int i = 0; i <= node->count; i++) {
            for (int j = 0; j < depth; j++) cout << "  ";
            cout << "child[" << i << "]: " << endl;
            printNode(node->children[i], depth + 1);
        }
    }
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
  
  cout << "=== Estructura ANTES de insertar 111 ===" << endl;
  // Necesitamos acceder al root, pero es privado... 
  // Vamos a hacer el test de otra forma
  cout << "String: " << btree->toString(" ") << endl;
  cout << "Height: " << btree->height() << endl;
  try {
    cout << "Max: " << btree->maxKey() << endl;
  } catch(...) {
    cout << "ERROR en maxKey" << endl;
  }
  
  cout << "\n=== Insertando 111 ===" << endl;
  btree->insert(111);
  
  cout << "\n=== Estructura DESPUÉS de insertar 111 ===" << endl;
  cout << "String: " << btree->toString(" ") << endl;
  cout << "Height: " << btree->height() << endl;
  try {
    cout << "Max: " << btree->maxKey() << endl;
  } catch(...) {
    cout << "ERROR en maxKey" << endl;
  }
  
  delete btree;
  return 0;
}
