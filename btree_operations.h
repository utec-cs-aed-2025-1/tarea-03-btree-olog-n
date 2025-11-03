#ifndef BTREE_OPERATIONS_H
#define BTREE_OPERATIONS_H

#include "btree.h"
#include <vector>
#include <iostream>

using namespace std;

// ==================== INSERT HELPER FUNCTIONS ====================

// Encuentra la posición del hijo donde debe ir la clave
template <typename TK>
int findChildPosition(Node<TK>* node, TK key) {
  int i = 0;
  while (i < node->count && key >= node->keys[i]) {
    i++;
  }
  return i;
}

// Inserta una clave en un nodo hoja manteniendo el orden
template <typename TK>
void insertInLeaf(Node<TK>* leaf, TK key) {
  int i = leaf->count - 1;
  
  // Desplazar las claves mayores a la derecha
  while (i >= 0 && key < leaf->keys[i]) {
    leaf->keys[i + 1] = leaf->keys[i];
    i--;
  }
  
  // Insertar la nueva clave
  leaf->keys[i + 1] = key;
  leaf->count++;
}

// Inserta una clave y un hijo derecho en un nodo padre
template <typename TK>
void insertInParent(Node<TK>* parent, int pos, TK key, Node<TK>* rightChild) {
  // Desplazar claves e hijos a la derecha
  for (int i = parent->count; i > pos; i--) {
    parent->keys[i] = parent->keys[i - 1];
    parent->children[i + 1] = parent->children[i];
  }
  
  // Insertar la nueva clave y el hijo derecho
  parent->keys[pos] = key;
  parent->children[pos + 1] = rightChild;
  parent->count++;
  parent->leaf = false;
}

// Divide un nodo lleno (con M claves) en dos nodos
template <typename TK>
void splitNode(Node<TK>* parent, int childIndex, int M) {
  Node<TK>* fullNode = parent->children[childIndex];
  Node<TK>* newNode = new Node<TK>(M);
  
  // Para split post-inserción: fullNode tiene M claves
  // Dividimos: left tendrá M/2 claves, midKey sube, right tendrá M - M/2 - 1 claves
  int midIndex = M / 2;
  TK midKey = fullNode->keys[midIndex];
  
  // Copiar claves a la derecha del midIndex al nuevo nodo (right)
  int newCount = 0;
  for (int i = midIndex + 1; i < fullNode->count; i++) {
    newNode->keys[newCount++] = fullNode->keys[i];
  }
  newNode->count = newCount;
  newNode->leaf = fullNode->leaf;
  
  // Si no es hoja, copiar también los hijos
  if (!fullNode->leaf) {
    // El nuevo nodo recibe los hijos desde midIndex+1 hasta el final
    for (int i = 0; i <= newCount; i++) {
      newNode->children[i] = fullNode->children[midIndex + 1 + i];
    }
    // Limpiar los punteros no usados en fullNode
    for (int i = midIndex + 1; i <= fullNode->count; i++) {
      fullNode->children[i] = nullptr;
    }
  }
  
  // Actualizar el count del nodo izquierdo (solo las claves antes de midIndex)
  fullNode->count = midIndex;
  
  // Insertar la clave media y el nuevo nodo en el padre
  insertInParent(parent, childIndex, midKey, newNode);
}

// Función recursiva de inserción
template <typename TK>
void insertRecursiveHelper(Node<TK>* node, TK key, int M) {
  if (node->leaf) {
    // Insertar en hoja
    insertInLeaf(node, key);
    return;
  }
  
  // Encontrar el hijo apropiado
  int pos = findChildPosition(node, key);
  
  // Insertar recursivamente en el hijo
  insertRecursiveHelper(node->children[pos], key, M);
  
  // Después de insertar, verificar si el hijo tiene overflow
  if (node->children[pos]->count == M) {
    splitNode(node, pos, M);
  }
}

// ==================== INSERT ====================

template <typename TK>
class BTree;  // Forward declaration

template <typename TK>
void insert(BTree<TK>* tree, TK key) {
  if (tree == nullptr) {
    return;
  }
  tree->insert(key);
}

// ==================== BUILD FROM ORDERED VECTOR ====================

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

// ==================== RANGE SEARCH ====================

template <typename TK>
void rangeSearchRecursive(Node<TK>* node, TK begin, TK end, vector<TK>& result) {
  if (node == nullptr) {
    return;
  }
  
  // Recorrer el nodo en orden
  int i = 0;
  
  // Procesar claves e hijos en orden
  for (i = 0; i < node->count; i++) {
    // Visitar hijo izquierdo si no es hoja
    if (!node->leaf && node->keys[i] >= begin) {
      rangeSearchRecursive(node->children[i], begin, end, result);
    }
    
    // Agregar la clave si está en el rango
    if (node->keys[i] >= begin && node->keys[i] <= end) {
      result.push_back(node->keys[i]);
    }
    
    // Si ya pasamos el final del rango, no seguir
    if (node->keys[i] > end) {
      return;
    }
  }
  
  // Visitar el último hijo (más a la derecha)
  if (!node->leaf) {
    rangeSearchRecursive(node->children[i], begin, end, result);
  }
}

// ==================== CHECK PROPERTIES ====================

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

// ==================== REMOVE HELPER FUNCTIONS ====================

// Encuentra el índice de la primera clave >= key
template <typename TK>
int findKeyIndex(Node<TK>* node, TK key) {
  int i = 0;
  while (i < node->count && node->keys[i] < key) {
    i++;
  }
  return i;
}

// Elimina una clave de un nodo hoja
template <typename TK>
void removeFromLeaf(Node<TK>* leaf, int idx) {
  for (int i = idx + 1; i < leaf->count; i++) {
    leaf->keys[i - 1] = leaf->keys[i];
  }
  leaf->count--;
}

// Obtiene el predecesor
template <typename TK>
TK getPredecessor(Node<TK>* node, int idx) {
  Node<TK>* current = node->children[idx];
  while (!current->leaf) {
    current = current->children[current->count];
  }
  return current->keys[current->count - 1];
}

// Obtiene el sucesor
template <typename TK>
TK getSuccessor(Node<TK>* node, int idx) {
  Node<TK>* current = node->children[idx + 1];
  while (!current->leaf) {
    current = current->children[0];
  }
  return current->keys[0];
}

// Merge de dos nodos
template <typename TK>
void merge(Node<TK>* node, int idx) {
  Node<TK>* child = node->children[idx];
  Node<TK>* sibling = node->children[idx + 1];
  
  // Bajar la clave del padre al hijo izquierdo
  child->keys[child->count] = node->keys[idx];
  
  // Copiar las claves del hermano derecho
  for (int i = 0; i < sibling->count; i++) {
    child->keys[child->count + 1 + i] = sibling->keys[i];
  }
  
  // Copiar los hijos del hermano si no es hoja
  if (!child->leaf) {
    for (int i = 0; i <= sibling->count; i++) {
      child->children[child->count + 1 + i] = sibling->children[i];
    }
  }
  
  child->count += sibling->count + 1;
  
  // Desplazar las claves del padre
  for (int i = idx + 1; i < node->count; i++) {
    node->keys[i - 1] = node->keys[i];
  }
  
  // Desplazar los punteros de hijos
  for (int i = idx + 2; i <= node->count; i++) {
    node->children[i - 1] = node->children[i];
  }
  
  node->count--;
  delete sibling;
}

// Tomar prestado del hermano izquierdo
template <typename TK>
void borrowFromPrev(Node<TK>* node, int childIdx, int M) {
  Node<TK>* child = node->children[childIdx];
  Node<TK>* sibling = node->children[childIdx - 1];
  
  // Desplazar todas las claves en child a la derecha
  for (int i = child->count - 1; i >= 0; i--) {
    child->keys[i + 1] = child->keys[i];
  }
  
  // Mover hijos si no es hoja
  if (!child->leaf) {
    for (int i = child->count; i >= 0; i--) {
      child->children[i + 1] = child->children[i];
    }
  }
  
  // Bajar la clave del padre
  child->keys[0] = node->keys[childIdx - 1];
  
  // Subir la última clave del hermano al padre
  node->keys[childIdx - 1] = sibling->keys[sibling->count - 1];
  
  // Mover el último hijo del hermano
  if (!child->leaf) {
    child->children[0] = sibling->children[sibling->count];
  }
  
  child->count++;
  sibling->count--;
}

// Tomar prestado del hermano derecho
template <typename TK>
void borrowFromNext(Node<TK>* node, int childIdx) {
  Node<TK>* child = node->children[childIdx];
  Node<TK>* sibling = node->children[childIdx + 1];
  
  // Bajar la clave del padre
  child->keys[child->count] = node->keys[childIdx];
  
  // Mover el primer hijo del hermano si no es hoja
  if (!child->leaf) {
    child->children[child->count + 1] = sibling->children[0];
  }
  
  // Subir la primera clave del hermano al padre
  node->keys[childIdx] = sibling->keys[0];
  
  // Desplazar todas las claves del hermano a la izquierda
  for (int i = 1; i < sibling->count; i++) {
    sibling->keys[i - 1] = sibling->keys[i];
  }
  
  // Desplazar los hijos
  if (!sibling->leaf) {
    for (int i = 1; i <= sibling->count; i++) {
      sibling->children[i - 1] = sibling->children[i];
    }
  }
  
  child->count++;
  sibling->count--;
}

// Asegurar que un hijo tenga suficientes claves antes de descender
template <typename TK>
void fill(Node<TK>* node, int idx, int M) {
  // Si el hermano izquierdo tiene claves extras
  if (idx != 0 && node->children[idx - 1]->count >= (M + 1) / 2) {
    borrowFromPrev(node, idx, M);
  }
  // Si el hermano derecho tiene claves extras
  else if (idx != node->count && node->children[idx + 1]->count >= (M + 1) / 2) {
    borrowFromNext(node, idx);
  }
  // Merge con hermano
  else {
    if (idx != node->count) {
      merge(node, idx);
    } else {
      merge(node, idx - 1);
    }
  }
}

// Eliminar de un nodo interno (no hoja)
template <typename TK>
void removeFromNonLeaf(Node<TK>* node, int idx, int M, int& n);

// Función recursiva principal de eliminación
template <typename TK>
void removeRecursive(Node<TK>* node, TK key, int M, int& n) {
  int idx = findKeyIndex(node, key);
  
  if (idx < node->count && node->keys[idx] == key) {
    // Caso 1: La clave está en este nodo
    if (node->leaf) {
      removeFromLeaf(node, idx);
      n--; // Decrementar solo cuando eliminamos de una hoja
    } else {
      removeFromNonLeaf(node, idx, M, n);
    }
  } else {
    // Caso 2: La clave debe estar en un subárbol
    if (node->leaf) {
      // La clave no existe en el árbol
      return;
    }
    
    bool isInLastChild = (idx == node->count);
    
    // Asegurar que el hijo tenga suficientes claves
    if (node->children[idx]->count < (M + 1) / 2) {
      fill(node, idx, M);
    }
    
    // Después de fill, la posición puede haber cambiado
    if (isInLastChild && idx > node->count) {
      removeRecursive(node->children[idx - 1], key, M, n);
    } else {
      removeRecursive(node->children[idx], key, M, n);
    }
  }
}

// Eliminar de un nodo interno (no hoja)
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
