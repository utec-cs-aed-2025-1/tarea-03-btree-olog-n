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

template <typename T>
struct Stk_Node{
  private: 
  T data;
  Stk_Node* next;

  public:
  Stk_Node(){}
  Stk_Node(T e): data(e){}
  Stk_Node(T e, Stk_Node* n): data(e), next(n){}

  ~Stk_Node(){
    delete next; 
  }
};

template <typename T>
struct Stack {
    Stk_Node<T>* top;
    Stack() : top(nullptr) {};
    Stack(T val) : top(new Stk_Node<T>(val)) {};
    
    bool isEmpty(){
        return (top == nullptr);
    }

    ~Stack(){
        while (!isEmpty()){
            this->pop();
        }
    }
    
    void push(T newData){
        Stk_Node<T>* nuevo = new Stk_Node<T>(newData);
        nuevo->next = top;
        top = nuevo;
    }

    void pop(){
        if (isEmpty()){
            throw std::runtime_error("error");
        } else {
            Stk_Node<T>* temp = top;
            top = temp->next;
            delete temp;
        }
    }

    T topM(){
        if (isEmpty()){
            throw std::runtime_error("error");
        }
        else {
            return this->top->data; 
        }
    }
    
};


template <typename TK>
class Iterator_BTree{ //Solo In order 
  private: 
    Node<TK>* current; 
    Stack<Node<TK>*> stack_nodo; // Para InOrder
    Stack<TK> stack_index; 
    int index; 

  public: 

    Iterator_BTree(): current(nullptr){}

    Iterator_BTree(Node<TK>* node): current(node){ 
      Node<TK>* temp = node;
      while (temp != nullptr) {
        stack_iter.push(temp);
        temp = temp->left;
      }
      ++(*this); //Current se convierte en el menor elemento
      index = 0;
    }
  
  Iterator_BTree<TK>& operator++(){ //In order, probar nuevo algoritmo
    if(current == nullptr){
      return this; 
    }
    
    if (stack_index.isEmpty() && stack_nodo.isEmpty()) {
      current = nullptr;
      return this;
    }

    this->current = stack_nodo.topM();
    this->index = stack_index.topM();
    stack_nodo.pop();
    stack_index.pop();
    if(index != this->current->count - 1){
      stack_nodo.push(current);
      stack_index.push(index+1);
    }
    Node<TK>* temp = current->children[index + 1]; 
    
    while (temp != nullptr) {
      stack_nodo.push(temp);
      stack_index.push(0);
      temp = temp->children[0];
    }
  }
};

#endif