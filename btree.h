#ifndef BTree_H
#define BTree_H
#include <iostream>
#include "node.h"

using namespace std;

template <typename TK>
class BTree {
 private:
  Node<TK>* root;
  int M;  // grado u orden del arbol
  int n; // total de elementos en el arbol 

 public:
  BTree(int _M) : root(nullptr), M(_M), n(0) {}

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
  } //Revisar overflow, casos excepcionales, nos imaginamps el algoritmo


  //---------------------------------------------------------------------------

  TK minKey();  // minimo valor de la llave en el arbol
  TK maxKey();  // maximo valor de la llave en el arbol
  void clear(); // eliminar todos lo elementos del arbol
  int size(); // retorna el total de elementos insertados  
  
  //-----------------------------------------------------------------------------------



  


  private:

  // Construya un árbol B a partir de un vector de elementos ordenados
  static BTree* build_from_ordered_vector(vector<T> elements);
  // Verifique las propiedades de un árbol B
  bool check_properties();


  string toStringRecursive(Node<TK>* node, string sep){
    if(node == nullptr){
      return "";
    }

    string ac_str = "";
    for (int i=0; i<node->count; i++){
      ac_str = ac_str + toStringRecursive(node->children[i], sep) + sep;
      ac_str = ac_str + (string)node->keys[i] + sep;
    }
    ac_str = ac_str + toStringRecursive(node->children[node->count], sep);
    
    return ac_str; 
  }

  bool searchRecursive(Node<TK>* node,T key){
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
          // Recalculamosx la posición después del split
          pos = findChildPosition(node, key);
      }
      insertRecursive(node->children[pos], key);
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


  ~BTree();     // liberar memoria
};

#endif