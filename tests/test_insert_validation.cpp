#include <iostream>
#include "btree.h"

using namespace std;

bool testInsertWithM(int M, int* numbers, int count, const string& label) {
    cout << "\n=== Test: " << label << " (M=" << M << ") ===" << endl;
    
    BTree<int>* btree = new BTree<int>(M);
    
    // Insertar elementos
    for (int i = 0; i < count; i++) {
        btree->insert(numbers[i]);
    }
    
    cout << "Elementos insertados: " << count << endl;
    cout << "Tamaño reportado: " << btree->size() << endl;
    cout << "Altura: " << btree->height() << endl;
    
    // Verificar que todos los elementos se insertaron
    bool allFound = true;
    for (int i = 0; i < count; i++) {
        if (!btree->search(numbers[i])) {
            cout << "ERROR: No se encontró " << numbers[i] << endl;
            allFound = false;
        }
    }
    
    // Verificar min y max
    bool minMaxOk = true;
    try {
        int min = btree->minKey();
        int max = btree->maxKey();
        
        int expectedMin = numbers[0];
        int expectedMax = numbers[0];
        for (int i = 1; i < count; i++) {
            if (numbers[i] < expectedMin) expectedMin = numbers[i];
            if (numbers[i] > expectedMax) expectedMax = numbers[i];
        }
        
        if (min != expectedMin || max != expectedMax) {
            cout << "ERROR: Min/Max incorrectos. Esperado: " << expectedMin << "/" << expectedMax 
                 << ", Obtenido: " << min << "/" << max << endl;
            minMaxOk = false;
        } else {
            cout << "Min: " << min << ", Max: " << max << " ✓" << endl;
        }
    } catch (...) {
        cout << "ERROR: Excepción al obtener min/max" << endl;
        minMaxOk = false;
    }
    
    // Verificar tamaño
    bool sizeOk = (btree->size() == count);
    if (!sizeOk) {
        cout << "ERROR: Tamaño incorrecto" << endl;
    }
    
    string resultado = btree->toString(" ");
    cout << "Árbol: " << resultado << endl;
    
    delete btree;
    
    bool success = allFound && minMaxOk && sizeOk;
    cout << (success ? "✓ PASÓ" : "✗ FALLÓ") << endl;
    
    return success;
}

int main() {
    cout << "=== Validación de Insert Post-Inserción ===" << endl;
    
    int allPassed = 0;
    int totalTests = 0;
    
    // Test 1: M=3 con secuencia problemática original
    int seq1[] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
    totalTests++;
    if (testInsertWithM(3, seq1, 10, "Secuencia problemática original")) allPassed++;
    
    // Test 2: M=3 con secuencia ordenada
    int seq2[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    totalTests++;
    if (testInsertWithM(3, seq2, 10, "Secuencia ordenada ascendente")) allPassed++;
    
    // Test 3: M=3 con secuencia descendente
    int seq3[] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
    totalTests++;
    if (testInsertWithM(3, seq3, 10, "Secuencia ordenada descendente")) allPassed++;
    
    // Test 4: M=5 con más elementos
    int seq4[] = {15, 25, 35, 5, 45, 55, 1, 20, 40, 50, 30, 10, 60, 70, 65};
    totalTests++;
    if (testInsertWithM(5, seq4, 15, "M=5 con 15 elementos")) allPassed++;
    
    // Test 5: M=4 (par)
    int seq5[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    totalTests++;
    if (testInsertWithM(4, seq5, 9, "M=4 (par) con 9 elementos")) allPassed++;
    
    // Test 6: M=3 con pocos elementos
    int seq6[] = {50, 30, 70};
    totalTests++;
    if (testInsertWithM(3, seq6, 3, "M=3 con 3 elementos")) allPassed++;
    
    // Test 7: M=7 con muchos elementos
    int seq7[] = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93};
    totalTests++;
    if (testInsertWithM(7, seq7, 15, "M=7 con 15 elementos")) allPassed++;
    
    cout << "\n================================================" << endl;
    cout << "Resultados: " << allPassed << "/" << totalTests << " tests pasados" << endl;
    
    if (allPassed == totalTests) {
        cout << "✓ TODOS LOS TESTS PASARON - Insert funciona correctamente!" << endl;
    } else {
        cout << "✗ ALGUNOS TESTS FALLARON" << endl;
    }
    
    return (allPassed == totalTests) ? 0 : 1;
}
