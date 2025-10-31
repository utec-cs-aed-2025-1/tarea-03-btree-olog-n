#ifndef NODE_H
#define NODE_H

using namespace std;

template <typename TK>
struct Node {
  // array de keys
  TK* keys;
  // array de punteros a hijos
  Node** children;
  // cantidad de keys
  int count;
  // indicador de nodo hoja
  bool leaf;

  Node() : keys(nullptr), children(nullptr), count(0) {}
  Node(int M) {
    keys = new TK[M - 1];
    children = new Node<TK>*[M];
    count = 0;
    leaf = true;
  }

  void killSelf() {
    if (children != nullptr) {
      int n =count + 1;
      for (int i = 0; i < n; i++) {
        if (children[i] != nullptr) {
          children[i]-> killSelf();
        }
      }
      delete[] children;
      children = nullptr;
    }

    if (keys != nullptr) {
      delete[] keys;
      keys = nullptr;
    }

    delete this;
  }
};

#endif