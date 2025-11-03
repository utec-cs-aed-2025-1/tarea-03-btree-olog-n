#ifndef BTREE_OPERATIONS_H
#define BTREE_OPERATIONS_H

#include "btree.h"
#include <vector>
#include <iostream>

using namespace std;

// INSERT HELPER FUNCTIONS

template <typename TK>
int findChildPosition(Node<TK>* node, TK key) {
  int i = 0;
  while (i < node->count && key >= node->keys[i]) {
    i++;
  }
  return i;
}

template <typename TK>
void insertInLeaf(Node<TK>* leaf, TK key) {
  int i = leaf->count - 1;
  
  while (i >= 0 && key < leaf->keys[i]) {
    leaf->keys[i + 1] = leaf->keys[i];
    i--;
  }
  
  leaf->keys[i + 1] = key;
  leaf->count++;
}

template <typename TK>
void insertInParent(Node<TK>* parent, int pos, TK key, Node<TK>* rightChild) {
  for (int i = parent->count; i > pos; i--) {
    parent->keys[i] = parent->keys[i - 1];
    parent->children[i + 1] = parent->children[i];
  }
  
  parent->keys[pos] = key;
  parent->children[pos + 1] = rightChild;
  parent->count++;
  parent->leaf = false;
}







template <typename TK>
void splitNode(Node<TK>* parent, int childIndex, int M) {
  Node<TK>* fullNode = parent->children[childIndex];
  Node<TK>* newNode = new Node<TK>(M);
  
  int midIndex = M / 2;
  TK midKey = fullNode->keys[midIndex];
  
  int newCount = 0;
  for (int i = midIndex + 1; i < fullNode->count; i++) {
    newNode->keys[newCount++] = fullNode->keys[i];
  }
  newNode->count = newCount;
  newNode->leaf = fullNode->leaf;
  
  if (!fullNode->leaf) {
    for (int i = 0; i <= newCount; i++) {
      newNode->children[i] = fullNode->children[midIndex + 1 + i];
    }
    for (int i = midIndex + 1; i <= fullNode->count; i++) {
      fullNode->children[i] = nullptr;
    }
  }
  
  fullNode->count = midIndex;
  
  insertInParent(parent, childIndex, midKey, newNode);
}







template <typename TK>
void insertRecursiveHelper(Node<TK>* node, TK key, int M) {
  if (node->leaf) {
    insertInLeaf(node, key); 
    return;
  }
  
  int pos = findChildPosition(node, key);
  
  insertRecursiveHelper(node->children[pos], key, M);
  
  if (node->children[pos]->count == M) { //verificación de overflow
    splitNode(node, pos, M);
  }
}






template <typename TK>
class BTree;  // Forward declaration

template <typename TK>
void insert(BTree<TK>* tree, TK key) {
  if (tree == nullptr) {
    return;
  }
  tree->insert(key);
}






template <typename TK>
BTree<TK>* build_from_ordered_vector(vector<TK> elements, int M) {
  if (elements.empty()) {
    return new BTree<TK>(M);
  }
  
  BTree<TK>* tree = new BTree<TK>(M);
  for (const TK& elem : elements) {
    tree->insert(elem);
  }
  return tree;
}








template <typename TK>
void rangeSearchRecursive(Node<TK>* node, TK begin, TK end, vector<TK>& result) {
  if (node == nullptr) {
    return;
  }
  
  int i = 0;
  
  for (i = 0; i < node->count; i++) {
    if (!node->leaf && node->keys[i] >= begin) {
      rangeSearchRecursive(node->children[i], begin, end, result);
    }
    
    if (node->keys[i] >= begin && node->keys[i] <= end) {
      result.push_back(node->keys[i]);
    }
    
    if (node->keys[i] > end) {
      return;
    }
  }
  
  if (!node->leaf) {
    rangeSearchRecursive(node->children[i], begin, end, result);
  }
}









template <typename TK>
bool checkPropertiesRecursive(Node<TK>* node, int M, int level, int& leafLevel, bool isRoot) {
  if (node == nullptr) {
    return true;
  }
  
  // Propiedad 1: Todas las hojas deben estar al mismo nivel
  if (node->leaf) {
    if (leafLevel == -1) {
      leafLevel = level;
    } else if (level != leafLevel) {
      std::cout << "ERROR: Hojas a diferentes niveles (" << level << " vs " << leafLevel << ")" << std::endl;
      return false;
    }
  }
  
  // Propiedad 2: Número mínimo y máximo de claves
  int minKeys = isRoot ? 1 : (M + 1) / 2 - 1;
  int maxKeys = M - 1;  // Normalmente M-1, pero permitimos M temporalmente durante operaciones
  
  // Durante merge, un nodo puede temporalmente tener M claves
  // Solo verificamos que no exceda M
  if (node->count > M) {
    std::cout << "ERROR: Nodo con demasiadas claves (" << node->count << " > " << M << ")" << std::endl;
    return false;
  }
  
  // Verificar mínimo solo si no es raíz
  if (!isRoot && node->count < minKeys) {
    std::cout << "ERROR: Nodo con muy pocas claves (" << node->count << " < " << minKeys << ")" << std::endl;
    return false;
  }
  
  // Propiedad 3: Las claves deben estar ordenadas
  for (int i = 0; i < node->count - 1; i++) {
    if (node->keys[i] >= node->keys[i + 1]) {
      std::cout << "ERROR: Claves no ordenadas en nodo" << std::endl;
      return false;
    }
  }
  
  // Propiedad 4: Si no es hoja, debe tener count+1 hijos
  if (!node->leaf) {
    for (int i = 0; i <= node->count; i++) {
      if (node->children[i] == nullptr) {
        std::cout << "ERROR: Hijo nulo en nodo interno (índice " << i << ")" << std::endl;
        return false;
      }
    }
    
    // Verificar recursivamente cada hijo
    for (int i = 0; i <= node->count; i++) {
      if (!checkPropertiesRecursive(node->children[i], M, level + 1, leafLevel, false)) {
        return false;
      }
    }
  }
  
  return true;
}








template <typename TK>
int findKeyIndex(Node<TK>* node, TK key) {
  int i = 0;
  while (i < node->count && node->keys[i] < key) {
    i++;
  }
  return i;
}

template <typename TK>
void removeFromLeaf(Node<TK>* leaf, int idx) {
  for (int i = idx + 1; i < leaf->count; i++) {
    leaf->keys[i - 1] = leaf->keys[i];
  }
  leaf->count--;
}

template <typename TK>
TK getPredecessor(Node<TK>* node, int idx) {
  Node<TK>* current = node->children[idx];
  while (!current->leaf) {
    current = current->children[current->count];
  }
  return current->keys[current->count - 1];
}

template <typename TK>
TK getSuccessor(Node<TK>* node, int idx) {
  Node<TK>* current = node->children[idx + 1];
  while (!current->leaf) {
    current = current->children[0];
  }
  return current->keys[0];
}


template <typename TK>
void merge(Node<TK>* node, int idx) {
  Node<TK>* child = node->children[idx];
  Node<TK>* sibling = node->children[idx + 1];
  
  child->keys[child->count] = node->keys[idx];
  
  for (int i = 0; i < sibling->count; i++) {
    child->keys[child->count + 1 + i] = sibling->keys[i];
  }
  
  if (!child->leaf) {
    for (int i = 0; i <= sibling->count; i++) {
      child->children[child->count + 1 + i] = sibling->children[i];
    }
  }
  
  child->count += sibling->count + 1;
  
  for (int i = idx + 1; i < node->count; i++) {
    node->keys[i - 1] = node->keys[i];
  }
  
  for (int i = idx + 2; i <= node->count; i++) {
    node->children[i - 1] = node->children[i];
  }
  
  node->count--;
  delete sibling;
}


template <typename TK>
void borrowFromPrev(Node<TK>* node, int childIdx, int M) {
  Node<TK>* child = node->children[childIdx];
  Node<TK>* sibling = node->children[childIdx - 1];
  
  for (int i = child->count - 1; i >= 0; i--) {
    child->keys[i + 1] = child->keys[i];
  }
  
  if (!child->leaf) {
    for (int i = child->count; i >= 0; i--) {
      child->children[i + 1] = child->children[i];
    }
  }
  
  child->keys[0] = node->keys[childIdx - 1];
  
  node->keys[childIdx - 1] = sibling->keys[sibling->count - 1];
  
  if (!child->leaf) {
    child->children[0] = sibling->children[sibling->count];
  }
  
  child->count++;
  sibling->count--;
}

template <typename TK>
void borrowFromNext(Node<TK>* node, int childIdx) {
  Node<TK>* child = node->children[childIdx];
  Node<TK>* sibling = node->children[childIdx + 1];
  
  child->keys[child->count] = node->keys[childIdx];
  
  if (!child->leaf) {
    child->children[child->count + 1] = sibling->children[0];
  }
  
  node->keys[childIdx] = sibling->keys[0];
  
  for (int i = 1; i < sibling->count; i++) {
    sibling->keys[i - 1] = sibling->keys[i];
  }
  
  if (!sibling->leaf) {
    for (int i = 1; i <= sibling->count; i++) {
      sibling->children[i - 1] = sibling->children[i];
    }
  }
  
  child->count++;
  sibling->count--;
}

template <typename TK>
void fill(Node<TK>* node, int idx, int M) {
  if (idx != 0 && node->children[idx - 1]->count >= (M + 1) / 2) {
    borrowFromPrev(node, idx, M);
  }
  else if (idx != node->count && node->children[idx + 1]->count >= (M + 1) / 2) {
    borrowFromNext(node, idx);
  }
  else {
    if (idx != node->count) {
      merge(node, idx);
    } else {
      merge(node, idx - 1);
    }
  }
}

template <typename TK>
void removeFromNonLeaf(Node<TK>* node, int idx, int M, int& n);

template <typename TK>
void removeRecursive(Node<TK>* node, TK key, int M, int& n) {
  int idx = findKeyIndex(node, key);
  
  if (idx < node->count && node->keys[idx] == key) {
    // Caso 1: La clave está en este nodo
    if (node->leaf) {
      removeFromLeaf(node, idx);
      n--;
    } else {
      removeFromNonLeaf(node, idx, M, n);
    }
  } else {
    // Caso 2: La clave debe estar en un subárbol
    if (node->leaf) {
      return;
    }
    
    bool isInLastChild = (idx == node->count);
    
    if (node->children[idx]->count < (M + 1) / 2) {
      fill(node, idx, M);
    }
    
    if (isInLastChild && idx > node->count) {
      removeRecursive(node->children[idx - 1], key, M, n);
    } else {
      removeRecursive(node->children[idx], key, M, n);
    }
  }
}

template <typename TK>
void removeFromNonLeaf(Node<TK>* node, int idx, int M, int& n) {
  TK key = node->keys[idx];
  
  // Caso 2a: El hijo izquierdo tiene suficientes claves
  if (node->children[idx]->count >= (M + 1) / 2) {
    TK predecessor = getPredecessor(node, idx);
    node->keys[idx] = predecessor;
    removeRecursive(node->children[idx], predecessor, M, n);
  }
  // Caso 2b: El hijo derecho tiene suficientes claves
  else if (node->children[idx + 1]->count >= (M + 1) / 2) {
    TK successor = getSuccessor(node, idx);
    node->keys[idx] = successor;
    removeRecursive(node->children[idx + 1], successor, M, n);
  }
  // Caso 2c: Ambos hijos tienen el mínimo de claves -> merge
  else {
    merge(node, idx);
    removeRecursive(node->children[idx], key, M, n);
  }
}

#endif
