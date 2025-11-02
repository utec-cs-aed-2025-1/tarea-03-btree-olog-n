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

    if (root->count == M-1) {
        Node<TK>* newRoot = new Node<TK>(M);
        newRoot->leaf = false;
        newRoot->children[0] = root;
        split(newRoot, 0);
        root = newRoot;
    }
    
    insertRecursive(root, key);
    n++;  
  }

  void insert2(TK key) {
    if (root == nullptr) {
        root = new Node<TK>(M);
        root->keys[0] = key;
        root->count = 1;
        root->leaf = true;
        n = 1;
        return;
    }
    
    insertRecursive2(root, key, nullptr);
    n++;  
  }

  void remove(TK key);//elimina un elemento
  int height();//altura del arbol. Considerar altura 0 para arbol vacio
  
  string toString(const string& sep){ // recorrido inorder
    return toStringRecursive(root, sep);
  }  
  
  vector<TK> rangeSearch(TK begin, TK end){
    if(begin > end){
      vector<TK> v = {};
      return v;
    }
    
    //Search empieza aquíi
    vector<TK> ans; 
    Node<TK>* temp = root;
    bool found = false;
    
    while (temp != nullptr && !found)
    { 
      int i =0;
      while (i<temp->count && temp->keys[i] < begin)
      { 
        i++;
      }
      if(temp->keys[i] == begin){
        found = true; 
        stack_nodo.push(temp);  
        stack_index.push(i);
      }
      else if(i==temp->count){
        temp = temp->children[i]; //Bajas por hijo derecho
      }
      else{
        stack_nodo.push(temp);
        stack_index.push(i);

        temp = temp->children[i];
      }
    }

    // Se encontro o se encontro la posicion en donde deberia estar(en una hoja)
    // Inorder
    Node<TK>* current = stack_nodo.topM();
    int index = stack_index.topM();

    while (current->keys[index] <= end && !stack_nodo.isEmpty())
    {
      ans.push_back(current->keys[index]);

      stack_nodo.pop();
      stack_index.pop();
      if(index != this->current->count - 1){ 
        stack_nodo.push(current);
        stack_index.push(index+1);
      }
      temp = current->children[index + 1]; 
    
      while (temp != nullptr) {
        stack_nodo.push(temp);
        stack_index.push(0);
        temp = temp->children[0];
      }

      current = stack_nodo.topM();
      index = stack_index.topM();
    }

    return ans; 
  } 

  //---------------------------------------------------------------------------

  TK minKey();  // minimo valor de la llave en el arbol
  TK maxKey();  // maximo valor de la llave en el arbol
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
  static BTree* build_from_ordered_vector(vector<TK> elements);
  // Verifique las propiedades de un árbol B
  bool check_properties();
  
  
  private:
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
          insertInLeaf(node, key);
          return;
      }
      
      int pos = findChildPosition(node, key);
      
      if (node->children[pos]->count == M - 1) {
          split(node, pos);
          // Recalculamos x la posición después del split
          pos = findChildPosition(node, key);
      }
      insertRecursive(node->children[pos], key);
  }

  void insertRecursive2(Node<TK>* node, TK key, Node<TK>* parent) {
      if (node->leaf) {
        insertInLeaf(node, key);
      }
      else{
        int pos = findChildPosition(node, key);      
        insertRecursive2(node->children[pos], key, node);
      }

      if (node->count > M - 1) {
        split2(parent, node);
      }
      return; 
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
      
      int midIndex = M/2;  // Centro derecha para los M pares
      TK midKey = fullNode->keys[midIndex];
      
      int newCount = 0;
      for (int i = midIndex+1; i < fullNode->count; i++) {
          newNode->keys[newCount++] = fullNode->keys[i];
      }
      newNode->count = newCount;
      newNode->leaf = fullNode->leaf;
      
      if (!fullNode->leaf) {
          for (int i=0; i <= newCount; i++) {
              newNode->children[i] = fullNode->children[midIndex + 1 + i];
          }
      }
      
      fullNode->count = midIndex;
      
      insertInParent(parent, childIndex, midKey, newNode);
  }

  void split2(Node<TK>* parent, Node<TK>* node) {
      Node<TK>* fullNode = node; 
      Node<TK>* newNode = new Node<TK>(M);
      
      int midIndex = M/2;  // Centro derecha para los M pares
      TK midKey = fullNode->keys[midIndex];
      
      int newCount = 0;
      for (int i = midIndex+1; i < fullNode->count; i++) { //Transfiere los keys
          newNode->keys[newCount++] = fullNode->keys[i];
      }
      newNode->count = newCount;
      newNode->leaf = fullNode->leaf;
      
      if (!fullNode->leaf) { //Transfiere los hijos al nuevo nodo derecho 
          for (int i=0; i <= newCount; i++) {
              newNode->children[i] = fullNode->children[midIndex + 1 + i];
          }
      }
      
      fullNode->count = midIndex;
      
      insertInParent2(parent, node, midKey, newNode);
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

  void insertInParent2(Node<TK>* parent, Node<TK>* node, TK key, Node<TK>* rightChild) {
    int pos = -1;
    for (int i =0; i<parent->count; i++){
      if(parent->children[i] == node){
        pos = i;
      }
    }  
    
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