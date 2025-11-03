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

  void remove(TK key);  // Implementación en btree_operations.h
  
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
  
  vector<TK> rangeSearch(TK begin, TK end);  // Implementación en btree_operations.h


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
  

  static BTree* build_from_ordered_vector(vector<TK> elements, int M);  // Implementación en btree_operations.h


  bool check_properties();  // Implementación en btree_operations.h
  
  
  private:
  
  // Las funciones auxiliares de rangeSearch, check_properties y remove
  // están implementadas en btree_operations.h como funciones friend
  
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
          // Insertar en hoja 
          insertInLeaf(node, key);
          return;
      }
      
      // Encontrar el hijo apropiado
      int pos = findChildPosition(node, key);
      
      // Insertar recursivamente en el hijo
      insertRecursive(node->children[pos], key);
      
      // Después de insertar, verificar si el hijo tiene overflow
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
      
      while (i >= 0 && key < leaf->keys[i]) { 
          leaf->keys[i + 1] = leaf->keys[i];
          i--;
      }
      
      leaf->keys[i + 1] = key;
      leaf->count++;
  }

  void split(Node<TK>* parent, int childIndex) {
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
  
  // Friend functions para acceder a miembros privados desde btree_operations.h
  template<typename T> friend void rangeSearchRecursive(Node<T>*, T, T, vector<T>&);
  template<typename T> friend bool checkPropertiesRecursive(Node<T>*, int, int, int&, bool);
  template<typename T> friend int findKeyIndex(Node<T>*, T);
  template<typename T> friend void removeFromLeaf(Node<T>*, int);
  template<typename T> friend T getPredecessor(Node<T>*, int);
  template<typename T> friend T getSuccessor(Node<T>*, int);
  template<typename T> friend void merge(Node<T>*, int);
  template<typename T> friend void borrowFromPrev(Node<T>*, int, int);
  template<typename T> friend void borrowFromNext(Node<T>*, int);
  template<typename T> friend void fill(Node<T>*, int, int);
  template<typename T> friend void removeFromNonLeaf(Node<T>*, int, int, int&);
  template<typename T> friend void removeRecursive(Node<T>*, T, int, int&);
  template<typename T> friend BTree<T>* build_from_ordered_vector(vector<T>, int);
};

// Incluir las implementaciones de las funciones auxiliares
#include "btree_operations.h"


template <typename TK>
void BTree<TK>::remove(TK key) {
  if (root == nullptr) {
    return; // Árbol vacío
  }
  
  removeRecursive(root, key, M, n);
  
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

template <typename TK>
vector<TK> BTree<TK>::rangeSearch(TK begin, TK end) {
  vector<TK> result;
  if(begin > end || root == nullptr){
    return result;
  }
  
  rangeSearchRecursive(root, begin, end, result);
  return result;
}

template <typename TK>
bool BTree<TK>::check_properties() {
  if (root == nullptr) {
    return true;
  }
  
  int leafLevel = -1;
  return checkPropertiesRecursive(root, M, 0, leafLevel, true);
}

template <typename TK>
BTree<TK>* BTree<TK>::build_from_ordered_vector(vector<TK> elements, int M) {
  return ::build_from_ordered_vector(elements, M);
}

#endif
