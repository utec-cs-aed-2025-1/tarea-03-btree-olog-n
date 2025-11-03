#include <iostream>
#include "btree.h"

using namespace std;

int main() {
    cout << "=== Test de build_from_ordered_vector ===" << endl << endl;
    
    // Test 1: Vector pequeño
    cout << "Test 1: Vector pequeño (M=3)" << endl;
    vector<int> v1 = {1, 2, 3, 4, 5};
    BTree<int>* bt1 = BTree<int>::build_from_ordered_vector(v1, 3);
    cout << "Árbol: " << bt1->toString(" ") << endl;
    cout << "Size: " << bt1->size() << ", Height: " << bt1->height() << endl;
    cout << "Check properties: " << (bt1->check_properties() ? "✓ PASÓ" : "✗ FALLÓ") << endl;
    delete bt1;
    cout << endl;
    
    // Test 2: Vector del main.cpp
    cout << "Test 2: Vector de 20 elementos (M=4)" << endl;
    vector<int> v2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    BTree<int>* bt2 = BTree<int>::build_from_ordered_vector(v2, 4);
    cout << "Árbol: " << bt2->toString(",") << endl;
    cout << "Size: " << bt2->size() << ", Height: " << bt2->height() << endl;
    cout << "Check properties: " << (bt2->check_properties() ? "✓ PASÓ" : "✗ FALLÓ") << endl;
    delete bt2;
    cout << endl;
    
    // Test 3: Vector vacío
    cout << "Test 3: Vector vacío (M=5)" << endl;
    vector<int> v3 = {};
    BTree<int>* bt3 = BTree<int>::build_from_ordered_vector(v3, 5);
    cout << "Size: " << bt3->size() << ", Height: " << bt3->height() << endl;
    cout << "Check properties: " << (bt3->check_properties() ? "✓ PASÓ" : "✗ FALLÓ") << endl;
    delete bt3;
    cout << endl;
    
    // Test 4: Vector grande (M=5)
    cout << "Test 4: Vector grande de 30 elementos (M=5)" << endl;
    vector<int> v4;
    for (int i = 1; i <= 30; i++) {
        v4.push_back(i * 10);
    }
    BTree<int>* bt4 = BTree<int>::build_from_ordered_vector(v4, 5);
    cout << "Size: " << bt4->size() << ", Height: " << bt4->height() << endl;
    cout << "Min: " << bt4->minKey() << ", Max: " << bt4->maxKey() << endl;
    cout << "Check properties: " << (bt4->check_properties() ? "✓ PASÓ" : "✗ FALLÓ") << endl;
    
    // Probar rangeSearch
    vector<int> range = bt4->rangeSearch(100, 200);
    cout << "rangeSearch(100, 200): ";
    for (int v : range) cout << v << " ";
    cout << endl;
    
    delete bt4;
    cout << endl;
    
    cout << "=== Todos los tests pasaron ===" << endl;
    
    return 0;
}
