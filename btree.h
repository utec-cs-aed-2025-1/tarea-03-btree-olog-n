#ifndef BTree_H
#define BTree_H
#include <iostream>
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
  BTree(int _M) : root(nullptr), M(_M) {}

  bool search(TK key){ //indica si se encuentra o no un elemento
    return searchRecursive(root, key);
  }
  void insert(TK key);//inserta un elemento
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
    
    //Search
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
  } //Revisar overflow, casos excepcionales, imaginar algoritmo

  TK minKey();  // minimo valor de la llave en el arbol
  TK maxKey();  // maximo valor de la llave en el arbol
  void clear(); // eliminar todos lo elementos del arbol
  int size(); // retorna el total de elementos insertados  
  
  // Construya un árbol B a partir de un vector de elementos ordenados
  static BTree* build_from_ordered_vector(vector<T> elements);
  // Verifique las propiedades de un árbol B
  bool check_properties();

  private:
  string toStringRecursive(Node<T>* node, string sep){
    if(node == nullptr){
      return "";
    }

    string ac_str = "";
    for (int i=0; i<node->count; i++){
      ac_str = ac_str + toStringRecursive(node->children[i]) + sep;
      ac_str = ac_str + (string)node->keys[i] + sep;
    }
      ac_str = ac_str + toStringRecursive(node->children[node->count]);
    return ac_str; 
  }

  bool searchRecursive(Node<T>* node,T key){
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

  ~BTree();     // liberar memoria
};

#endif
