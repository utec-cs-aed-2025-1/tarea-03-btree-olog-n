#include <iostream>
#include "btree.h"

using namespace std;

int main() {
    cout << "=== Test de rangeSearch ===" << endl << endl;
    
    BTree<int>* btree = new BTree<int>(3);
    
    // Insertar elementos
    int vals[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    for (int i = 0; i < 11; i++) {
        btree->insert(vals[i]);
    }
    
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Tamaño: " << btree->size() << endl;
    cout << "Altura: " << btree->height() << endl << endl;
    
    // Test 1: Rango completo
    cout << "Test 1: rangeSearch(10, 80)" << endl;
    vector<int> r1 = btree->rangeSearch(10, 80);
    cout << "Resultado: ";
    for (int v : r1) cout << v << " ";
    cout << endl << endl;
    
    // Test 2: Rango medio
    cout << "Test 2: rangeSearch(25, 60)" << endl;
    vector<int> r2 = btree->rangeSearch(25, 60);
    cout << "Resultado: ";
    for (int v : r2) cout << v << " ";
    cout << endl << endl;
    
    // Test 3: Rango pequeño
    cout << "Test 3: rangeSearch(35, 45)" << endl;
    vector<int> r3 = btree->rangeSearch(35, 45);
    cout << "Resultado: ";
    for (int v : r3) cout << v << " ";
    cout << endl << endl;
    
    // Test 4: Rango con un solo elemento
    cout << "Test 4: rangeSearch(50, 50)" << endl;
    vector<int> r4 = btree->rangeSearch(50, 50);
    cout << "Resultado: ";
    for (int v : r4) cout << v << " ";
    cout << endl << endl;
    
    // Test 5: Rango vacío (begin > end)
    cout << "Test 5: rangeSearch(60, 40)" << endl;
    vector<int> r5 = btree->rangeSearch(60, 40);
    cout << "Resultado: ";
    for (int v : r5) cout << v << " ";
    cout << "(debería estar vacío)" << endl << endl;
    
    // Test 6: Rango fuera de los valores
    cout << "Test 6: rangeSearch(100, 200)" << endl;
    vector<int> r6 = btree->rangeSearch(100, 200);
    cout << "Resultado: ";
    for (int v : r6) cout << v << " ";
    cout << "(debería estar vacío)" << endl << endl;
    
    delete btree;
    
    cout << "=== Test de rangeSearch completado ===" << endl;
    
    return 0;
}
