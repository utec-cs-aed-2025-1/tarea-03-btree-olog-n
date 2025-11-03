#ifndef BTree_H
#define BTree_H
#include <iostream>
#include <sstream>
#include "node.h"
#include <vector>

using namespace std;

// Forward declarations de las funciones de btree_operations.h
template <typename TK> int findChildPosition(Node<TK>* node, TK key);
template <typename TK> void insertInLeaf(Node<TK>* leaf, TK key);
template <typename TK> void insertInParent(Node<TK>* parent, int pos, TK key, Node<TK>* rightChild);
template <typename TK> void splitNode(Node<TK>* parent, int childIndex, int M);
template <typename TK> void insertRecursiveHelper(Node<TK>* node, TK key, int M);
template <typename TK> void rangeSearchRecursive(Node<TK>* node, TK begin, TK end, vector<TK>& result);
template <typename TK> bool checkPropertiesRecursive(Node<TK>* node, int M, int level, int& leafLevel, bool isRoot);
template <typename TK> void removeRecursive(Node<TK>* node, TK key, int M, int& n);

template <typename TK>
class BTree {
 private:
  Node<TK>* root;
  int M;  // grado u orden del arbol
  int n; // total de elementos en el arbol 

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

    // Insertar usando el enfoque post-inserción de btree_operations.h
    insertRecursiveHelper(root, key, M);
    
    // Si la raíz tiene overflow (M claves), dividirla
    if (root->count == M) {
        Node<TK>* newRoot = new Node<TK>(M);
        newRoot->leaf = false;
        newRoot->children[0] = root;
        newRoot->count = 0;
        
        // Dividir la raíz usando la función de btree_operations.h
        splitNode(newRoot, 0, M);
        root = newRoot;
    }
    
    n++;  
  }

  void remove(TK key) {
    if (root == nullptr) {
      return;
    }
    
    // Usar la función de btree_operations.h
    removeRecursive(root, key, M, n);
    
    // Si la raíz quedó vacía después de la eliminación
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
  
  int height(){//altura del arbol. Considerar altura 0 para arbol vacio
    return heightRecursive(root);
  }
  
  string toString(const string& sep){ // recorrido inorder
    string result = toStringRecursive(root, sep);
    // Eliminar el último separador si existe
    if (!result.empty() && result.size() >= sep.size()) {
      if (result.substr(result.size() - sep.size()) == sep) {
        result = result.substr(0, result.size() - sep.size());
      }
    }
    return result;
  }  
  
  vector<TK> rangeSearch(TK begin, TK end){
    vector<TK> result;
    if (begin > end || root == nullptr) {
      return result;
    }
    // Usar la función de btree_operations.h
    rangeSearchRecursive(root, begin, end, result);
    return result;
  } 


  TK minKey(){  // minimo valor de la llave en el arbol
    if (root == nullptr) {
      throw std::runtime_error("El arbol esta vacio");
    }
    Node<TK>* current = root;
    // Ir siempre al hijo más a la izquierda
    while (!current->leaf) {
      current = current->children[0];
    }
    // El primer key del nodo hoja más a la izquierda es el mínimo
    return current->keys[0];
  }
  
  TK maxKey(){  // maximo valor de la llave en el arbol
    if (root == nullptr) {
      throw std::runtime_error("El arbol esta vacio");
    }
    Node<TK>* current = root;
    // Ir siempre al hijo más a la derecha
    while (!current->leaf) {
      current = current->children[current->count];
    }
    // El último key del nodo hoja más a la derecha es el máximo
    return current->keys[current->count - 1];
  }
  
  void clear() { // Vaciar o eliminar todos los nodos del BTree
    clearRecursive(root);
    root = nullptr;
    n = 0;
  }

  int size() { // retorna total de elementos insertador
    return n;
  }
  
  //-----------------------------------------------------------------------------------


  
  // Construya un árbol B a partir de un vector de elementos ordenados
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
  
  // Verifique las propiedades de un árbol B
  bool check_properties() {
    if (root == nullptr) {
      return true;
    }
    int leafLevel = -1;
    // Usar la función de btree_operations.h
    return checkPropertiesRecursive(root, M, 0, leafLevel, true);
  }
  
  
  private:
  int heightRecursive(Node<TK>* node) {
    if (node == nullptr) {
      return 0;  // Árbol vacío tiene altura 0
    }
    
    if (node->leaf) {
      return 0;  // Un nodo hoja tiene altura 1
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

  public: 
  ~BTree() {
    clear();  // Assuming clear() properly deletes all nodes
  }
};

// Incluir las implementaciones de btree_operations.h después de la definición de la clase
#include "btree_operations.h"

#endif