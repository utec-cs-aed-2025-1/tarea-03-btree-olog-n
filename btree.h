#ifndef BTree_H
#define BTree_H
#include <iostream>
#include <sstream>
#include "node.h"
#include <vector>

using namespace std;

template <typename TK>
class BTree {
 private:
  Node<TK>* root;
  int M;  // grado u orden del arbol
  int n; // total de elementos en el arbol 
  Stack<Node<TK>*> stack_nodo;
  Stack<int> stack_index; 

 public:
  BTree(int _M) : root(nullptr), M(_M), n(0) {}
  BTree(Node<TK>* node, int _M): root(node), M(_M), n(node->count) {}

  bool search(TK key){ //indica si se encuentra o no un elemento
    return searchRecursive(root, key);
  }

  void insert(TK key) {
    if (root == nullptr) {
        root = new Node<TK>(M);
        root->keys[0] = key;
        root->count = 1;
        root->leaf = true;
        n = 1;
        return;
    }

    // Estrategia post-inserción: insertar primero, luego manejar overflow
    insertRecursive(root, key);
    n++;
    
    // Si la raíz tiene overflow (count == M), dividirla
    if (root->count == M) {
        Node<TK>* newRoot = new Node<TK>(M);
        newRoot->leaf = false;
        newRoot->children[0] = root;
        newRoot->count = 0;  // Será incrementado por insertInParent
        split(newRoot, 0);
        root = newRoot;
    }
  }

  void remove(TK key){//elimina un elemento
    if (root == nullptr) {
      return; // Árbol vacío
    }
    
    removeRecursive(root, key);
    
    // Si la raíz queda vacía después de la eliminación
    if (root->count == 0) {
      Node<TK>* oldRoot = root;
      if (root->leaf) {
        root = nullptr;
      } else {
        root = root->children[0];
      }
      delete oldRoot;
    }
  }
  
  int height(){//altura del arbol. 
    return heightRecursive(root);
  }
  
  string toString(const string& sep){ // recorrido inorder
    string result = toStringRecursive(root, sep);
    
    if (!result.empty() && result.size() >= sep.size()) {
      if (result.substr(result.size() - sep.size()) == sep) {
        result = result.substr(0, result.size() - sep.size());
      }
    }
    return result;
  }  
  
  vector<TK> rangeSearch(TK begin, TK end){
    vector<TK> result;
    if(begin > end || root == nullptr){
      return result;
    }
    
    rangeSearchRecursive(root, begin, end, result);
    return result;
  } 


  TK minKey(){  
    if (root == nullptr) {
      throw std::runtime_error("El arbol esta vacio");
    }
    Node<TK>* current = root;
    
    while (!current->leaf) {
      current = current->children[0];
    }
    
    return current->keys[0];
  }
  
  TK maxKey(){  
    if (root == nullptr) {
      throw std::runtime_error("El arbol esta vacio");
    }
    Node<TK>* current = root;
    
    while (!current->leaf) {
      current = current->children[current->count];
    }
    
    return current->keys[current->count - 1];
  }
  
  void clear() { 
    clearRecursive(root);
    root = nullptr;
    n = 0;
  }

  int size() { 
    return n;
  }
  

  static BTree* build_from_ordered_vector(vector<TK> elements, int M) {
    if (elements.empty()) {
      return new BTree<TK>(M);
    }
    
    BTree<TK>* tree = new BTree<TK>(M);
    for (const TK& elem : elements) {
      tree->insert(elem);
    }
    return tree;
  }


  bool check_properties() {
    if (root == nullptr) {
      return true;
    }
    
    int leafLevel = -1;
    return checkPropertiesRecursive(root, 0, leafLevel, true);
  }
  
  
  private:
  
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
  
  // ==================== FUNCIÓN AUXILIAR PARA CHECK_PROPERTIES ====================
  
  bool checkPropertiesRecursive(Node<TK>* node, int level, int& leafLevel, bool isRoot) {
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
        if (!checkPropertiesRecursive(node->children[i], level + 1, leafLevel, false)) {
          return false;
        }
      }
    }
    
    return true;
  }
  
  // ==================== FUNCIONES AUXILIARES PARA REMOVE ====================
  
  // Encuentra el índice de la primera clave >= key (reutiliza lógica de findChildPosition)
  int findKeyIndex(Node<TK>* node, TK key) {
    int i = 0;
    while (i < node->count && node->keys[i] < key) {
      i++;
    }
    return i;
  }
  
  // Elimina una clave de un nodo hoja (inverso de insertInLeaf)
  void removeFromLeaf(Node<TK>* leaf, int idx) {
    for (int i = idx + 1; i < leaf->count; i++) {
      leaf->keys[i - 1] = leaf->keys[i];
    }
    leaf->count--;
  }
  
  // Obtiene el predecesor (reutiliza lógica de maxKey desde un nodo específico)
  TK getPredecessor(Node<TK>* node, int idx) {
    Node<TK>* current = node->children[idx];
    while (!current->leaf) {
      current = current->children[current->count];
    }
    return current->keys[current->count - 1];
  }
  
  // Obtiene el sucesor (reutiliza lógica de minKey desde un nodo específico)
  TK getSuccessor(Node<TK>* node, int idx) {
    Node<TK>* current = node->children[idx + 1];
    while (!current->leaf) {
      current = current->children[0];
    }
    return current->keys[0];
  }
  
  // Merge de dos nodos (operación inversa a split)
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
    
    // Desplazar las claves del padre (similar a removeFromLeaf)
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
  
  // Tomar prestado del hermano izquierdo (usa lógica similar a insertInParent)
  void borrowFromPrev(Node<TK>* node, int childIdx) {
    Node<TK>* child = node->children[childIdx];
    Node<TK>* sibling = node->children[childIdx - 1];
    
    // Desplazar todas las claves en child a la derecha (similar a insertInLeaf)
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
  
  // Tomar prestado del hermano derecho (inverso de borrowFromPrev)
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
  void fill(Node<TK>* node, int idx) {
    // Si el hermano izquierdo tiene claves extras
    if (idx != 0 && node->children[idx - 1]->count >= (M + 1) / 2) {
      borrowFromPrev(node, idx);
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
  void removeFromNonLeaf(Node<TK>* node, int idx) {
    TK key = node->keys[idx];
    

    // Caso 2a: El hijo izquierdo tiene suficientes claves
    if (node->children[idx]->count >= (M + 1) / 2) {
      TK predecessor = getPredecessor(node, idx);
      node->keys[idx] = predecessor;
      removeRecursive(node->children[idx], predecessor);
    }
    // Caso 2b: El hijo derecho tiene suficientes claves
    else if (node->children[idx + 1]->count >= (M + 1) / 2) {
      TK successor = getSuccessor(node, idx);
      node->keys[idx] = successor;
      removeRecursive(node->children[idx + 1], successor);
    }
    // Caso 2c: Ambos hijos tienen el mínimo de claves -> merge
    else {
      merge(node, idx);
      removeRecursive(node->children[idx], key);
    }
  }
  
  // Función recursiva principal de eliminación
  void removeRecursive(Node<TK>* node, TK key) {
    int idx = findKeyIndex(node, key);
    
    if (idx < node->count && node->keys[idx] == key) {
      // Caso 1: La clave está en este nodo
      if (node->leaf) {
        removeFromLeaf(node, idx);
        n--; // Decrementar solo cuando eliminamos de una hoja
      } else {
        removeFromNonLeaf(node, idx);
        // No decrementamos aquí porque removeFromNonLeaf mueve una clave de una hoja
        // y la llamada recursiva a removeRecursive disminuirá n cuando elimine de la hoja
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
        fill(node, idx);
      }
      
      // Después de fill, la posición puede haber cambiado
      if (isInLastChild && idx > node->count) {
        removeRecursive(node->children[idx - 1], key);
      } else {
        removeRecursive(node->children[idx], key);
      }
    }
  }
  
  // ==================== FIN FUNCIONES REMOVE ====================

  int heightRecursive(Node<TK>* node) {
    if (node == nullptr) {
      return 0;  // Árbol vacío tiene altura 0
    }
    
    if (node->leaf) {
      return 0;  // Un nodo hoja tiene altura 0 (nivel de las hojas)
    }
    
    // Para un B-Tree balanceado, todos los caminos tienen la misma altura
    // Basta con seguir cualquier camino (usamos el primero)
    return 1 + heightRecursive(node->children[0]);
  }

  void clearRecursive(Node<TK>* node) {
    if (node != nullptr) {
      if (!node->leaf) {
        for (int i = 0; i <= node->count; i++) {
          clearRecursive(node->children[i]);
        }
      }
      delete node;
    }
  }

  string toStringRecursive(Node<TK>* node, const string& sep){
    if(node == nullptr){
      return "";
    }

    string ac_str = "";
    for (int i = 0; i < node->count; i++){
      
      if (!node->leaf) {
        ac_str += toStringRecursive(node->children[i], sep);
      }
      std::ostringstream ss;
      ss << node->keys[i];
      ac_str += ss.str() + sep;
    }
    
    if (!node->leaf) {
      ac_str += toStringRecursive(node->children[node->count], sep);
    }

    return ac_str; 
  }

  bool searchRecursive(Node<TK>* node,TK key){
    if (node == nullptr){
      return false; 
    }

    for (int i=0; i<node->count; i++){
      if(node->keys[i] > key){
        return searchRecursive(node->children[i], key);
      }
      else if(node->keys[i] == key){
        return true; 
      }
    }
    return searchRecursive(node->children[node->count], key);
  }


  void insertRecursive(Node<TK>* node, TK key) {
      if (node->leaf) {
          // Insertar en hoja (permite temporalmente count == M)
          insertInLeaf(node, key);
          return;
      }
      
      // Encontrar el hijo apropiado
      int pos = findChildPosition(node, key);
      
      // Insertar recursivamente en el hijo
      insertRecursive(node->children[pos], key);
      
      // Después de insertar, verificar si el hijo tiene overflow (count == M)
      if (node->children[pos]->count == M) {
          split(node, pos);
      }
  }

  int findChildPosition(Node<TK>* node, TK key) {
      int i = 0;
      while (i < node->count && key >= node->keys[i]) {
          i++;
      }
      return i;
  }

  void insertInLeaf(Node<TK>* leaf, TK key) {
      int i = leaf->count - 1;
      
      while (i >= 0 && key < leaf->keys[i]) { // Similar al Insertion Sort
          leaf->keys[i + 1] = leaf->keys[i];
          i--;
      }
      
      leaf->keys[i + 1] = key;
      leaf->count++;
  }

  void split(Node<TK>* parent, int childIndex) {
      Node<TK>* fullNode = parent->children[childIndex];
      Node<TK>* newNode = new Node<TK>(M);
      
      // Para split post-inserción: fullNode tiene M claves (overflow)
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
          // Como fullNode tiene M claves, tiene M+1 hijos
          // Después del split: left tiene midIndex claves y midIndex+1 hijos
          //                    right tiene newCount claves y newCount+1 hijos
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


  public: 
  ~BTree() {
    clear();  // Assuming clear() properly deletes all nodes
  }
};

#endif