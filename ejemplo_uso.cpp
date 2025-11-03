/*
 * Ejemplo de uso del B-Tree con funciones separadas
 * 
 * Las funciones rangeSearch, remove, check_properties y build_from_ordered_vector
 * están implementadas en btree_operations.h
 * 
 * Para usar el B-Tree, solo necesitas incluir btree.h, que automáticamente
 * incluye btree_operations.h con todas las implementaciones.
 */

#include <iostream>
#include "btree.h"

using namespace std;

int main() {
    // Crear un B-Tree de orden 3
    BTree<int>* btree = new BTree<int>(3);
    
    // Insertar elementos
    cout << "=== INSERCIÓN ===" << endl;
    int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int val : values) {
        btree->insert(val);
        cout << "Insertado: " << val << endl;
    }
    cout << "Árbol: " << btree->toString(" ") << endl;
    cout << "Altura: " << btree->height() << endl;
    
    // rangeSearch - implementada en btree_operations.h
    cout << "\n=== RANGE SEARCH ===" << endl;
    vector<int> range = btree->rangeSearch(6, 17);
    cout << "Elementos en rango [6, 17]: ";
    for (int val : range) {
        cout << val << " ";
    }
    cout << endl;
    
    // check_properties - implementada en btree_operations.h
    cout << "\n=== CHECK PROPERTIES ===" << endl;
    if (btree->check_properties()) {
        cout << "✓ El árbol cumple todas las propiedades de un B-Tree" << endl;
    } else {
        cout << "✗ El árbol NO cumple las propiedades de un B-Tree" << endl;
    }
    
    // remove - implementada en btree_operations.h
    cout << "\n=== REMOVE ===" << endl;
    cout << "Eliminando 6..." << endl;
    btree->remove(6);
    cout << "Árbol después de eliminar 6: " << btree->toString(" ") << endl;
    
    if (btree->check_properties()) {
        cout << "✓ El árbol sigue siendo válido después del remove" << endl;
    }
    
    // build_from_ordered_vector - implementada en btree_operations.h
    cout << "\n=== BUILD FROM ORDERED VECTOR ===" << endl;
    vector<int> ordered = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BTree<int>* btree2 = BTree<int>::build_from_ordered_vector(ordered, 4);
    cout << "Árbol construido desde vector ordenado: " << btree2->toString(",") << endl;
    cout << "Altura: " << btree2->height() << endl;
    
    if (btree2->check_properties()) {
        cout << "✓ El árbol construido es válido" << endl;
    }
    
    // Limpiar
    delete btree;
    delete btree2;
    
    return 0;
}
