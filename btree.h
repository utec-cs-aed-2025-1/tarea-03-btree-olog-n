#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include "node.h"
using namespace std;

template <typename TK>
class BTree {
 private:
  Node<TK>* root;
  int M;  // grado u orden del arbol
  int n;  // total de elementos en el arbol 

 public:
  BTree(int _M) : root(nullptr), M(_M), n(0) {}

  bool search(TK key) { return searchRecursive(root, key); }
  void insert(TK key);
  void remove(TK key) { root = removeRecursive(root, key); }
  int height() const { return height(root); }
  string toString(const string& sep) { return toStringRecursive(root, sep); }
  TK minKey();
  TK maxKey();
  void clear();
  int size() const { return n; }

  static BTree* build_from_ordered_vector(const vector<TK>& elements, int order);
  bool check_properties();

 private:

  Node<TK>* insertRecursive(Node<TK>* node, TK key, TK& promoted);
  Node<TK>* split(Node<TK>* node, TK& promoted);
  int findChildPosition(Node<TK>* node, TK key);
  void insertInLeaf(Node<TK>* leaf, TK key);
  bool searchRecursive(Node<TK>* node, TK key);
  string toStringRecursive(Node<TK>* node, const string& sep) const;
  int height(Node<TK>* node) const;
  void clear(Node<TK>* node);
  Node<TK>* removeRecursive(Node<TK>* node, TK key);
  void collect_keys(Node<TK>* node, vector<TK>& v);
};



template <typename TK>
void BTree<TK>::insert(TK key) {
  if (root == nullptr) {
    root = new Node<TK>(M);
    root->keys[0] = key;
    root->count = 1;
    root->leaf = true;
    n = 1;
    return;
  }

  TK promoted;
  Node<TK>* newChild = insertRecursive(root, key, promoted);
  if (newChild != nullptr) {
    Node<TK>* newRoot = new Node<TK>(M);
    newRoot->keys[0] = promoted;
    newRoot->children[0] = root;
    newRoot->children[1] = newChild;
    newRoot->count = 1;
    newRoot->leaf = false;
    root = newRoot;
  }
  n++;
}

template <typename TK>
Node<TK>* BTree<TK>::insertRecursive(Node<TK>* node, TK key, TK& promoted) {
    if (node->leaf) {
        insertInLeaf(node, key);
        if (node->count == M - 1) {  // porsi se llenó
            return split(node, promoted);
        }
        return nullptr;
    }

    // Nodo interno
    int pos = findChildPosition(node, key);
    TK childPromoted;
    Node<TK>* newChild = insertRecursive(node->children[pos], key, childPromoted);

    if (newChild == nullptr)
        return nullptr;

    for (int i = node->count; i > pos; --i) {
        node->keys[i] = node->keys[i - 1];
        node->children[i + 1] = node->children[i];
    }

    node->keys[pos] = childPromoted;
    node->children[pos + 1] = newChild;
    node->count++;

    if (node->count == M - 1)
        return split(node, promoted);

    return nullptr;
}


template <typename TK>
Node<TK>* BTree<TK>::split(Node<TK>* node, TK& promoted) {
    int mid = (M - 1) / 2; // punto medio (mejor definido)
    Node<TK>* newNode = new Node<TK>(M);

    promoted = node->keys[mid];
    newNode->leaf = node->leaf;

    // mover claves del lado derecho
    int j = 0;
    for (int i = mid + 1; i < node->count; ++i)
        newNode->keys[j++] = node->keys[i];

    // si no es hoja, mover también los punteros a hijos
    if (!node->leaf) {
        for (int i = mid + 1; i <= node->count; ++i)
            newNode->children[i - (mid + 1)] = node->children[i];
    }

    newNode->count = node->count - mid - 1;
    node->count = mid;
    return newNode;
}


template <typename TK>
int BTree<TK>::findChildPosition(Node<TK>* node, TK key) {
  int i = 0;
  while (i < node->count && key >= node->keys[i]) i++;
  return i;
}

template <typename TK>
void BTree<TK>::insertInLeaf(Node<TK>* leaf, TK key) {
  int i = leaf->count - 1;
  while (i >= 0 && key < leaf->keys[i]) {
    leaf->keys[i + 1] = leaf->keys[i];
    i--;
  }
  leaf->keys[i + 1] = key;
  leaf->count++;
}

template <typename TK>
bool BTree<TK>::searchRecursive(Node<TK>* node, TK key) {
  if (node == nullptr) return false;
  int i = 0;
  while (i < node->count && key > node->keys[i]) i++;
  if (i < node->count && key == node->keys[i]) return true;
  return searchRecursive(node->children[i], key);
}

template <typename TK>
string BTree<TK>::toStringRecursive(Node<TK>* node, const string& sep) const {
  if (node == nullptr) return "";
  string s = "";
  for (int i = 0; i < node->count; i++) {
    s += toStringRecursive(node->children[i], sep);
    if (!s.empty() && s.back() != sep[0]) s += sep;
    s += to_string(node->keys[i]);
    if (i < node->count - 1) s += sep;
  }
  if (!node->leaf) {
    if (!s.empty() && s.back() != sep[0]) s += sep;
    s += toStringRecursive(node->children[node->count], sep);
  }
  return s;
}

template <typename TK>
int BTree<TK>::height(Node<TK>* node) const {
    if (node == nullptr) return 0;
    int h = 0;
    while (node != nullptr) {
        h++;
        if (node->leaf) break;
        node = node->children[0];
    }
    return h;
}


template <typename TK>
TK BTree<TK>::minKey() {
  if (root == nullptr) throw runtime_error("Empty tree");
  Node<TK>* node = root;
  while (!node->leaf)
    node = node->children[0];
  return node->keys[0];
}

template <typename TK>
TK BTree<TK>::maxKey() {
    if (root == nullptr) throw runtime_error("Empty tree");
    Node<TK>* node = root;
    while (!node->leaf)
        node = node->children[node->count];
    return node->keys[node->count - 1];
}


template <typename TK>
void BTree<TK>::clear(Node<TK>* node) {
  if (node == nullptr) return;
  if (!node->leaf) {
    for (int i = 0; i <= node->count; ++i)
      clear(node->children[i]);
  }
  delete node;
}

template <typename TK>
void BTree<TK>::clear() {
  clear(root);
  root = nullptr;
  n = 0;
}

template <typename TK>
Node<TK>* BTree<TK>::removeRecursive(Node<TK>* node, TK key) {
  if (node == nullptr) return nullptr;
  int i = 0;
  while (i < node->count && key > node->keys[i]) i++;

  if (i < node->count && key == node->keys[i]) {
    if (node->leaf) {
      for (int j = i; j < node->count - 1; ++j)
        node->keys[j] = node->keys[j + 1];
      node->count--;
      n--;
      return node;
    } else {
      Node<TK>* pred = node->children[i];
      while (!pred->leaf)
        pred = pred->children[pred->count];
      node->keys[i] = pred->keys[pred->count - 1];
      node->children[i] = removeRecursive(node->children[i], node->keys[i]);
    }
  } else if (!node->leaf)
    node->children[i] = removeRecursive(node->children[i], key);
  return node;
}

template <typename TK>
void BTree<TK>::collect_keys(Node<TK>* node, vector<TK>& v) {
  if (node == nullptr) return;
  for (int i = 0; i < node->count; ++i) {
    collect_keys(node->children[i], v);
    v.push_back(node->keys[i]);
  }
  collect_keys(node->children[node->count], v);
}

template <typename TK>
bool BTree<TK>::check_properties() {
  vector<TK> v;
  collect_keys(root, v);
  for (int i = 1; i < (int)v.size(); ++i) {
    if (v[i - 1] > v[i]) return false;
  }
  return true;
}

template <typename TK>
BTree<TK>* BTree<TK>::build_from_ordered_vector(const vector<TK>& elements, int order) {
  BTree<TK>* tree = new BTree<TK>(order);
  for (const TK& e : elements) tree->insert(e);
  return tree;
}

#endif
