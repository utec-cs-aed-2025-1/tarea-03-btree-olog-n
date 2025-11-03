# Estructura del Proyecto B-Tree

## Archivos Principales

### Archivos de Cabecera (Headers)

- **`node.h`**: Define la estructura `Node<TK>` y la clase auxiliar `Stack`
- **`btree.h`**: Define la clase `BTree<TK>` con las funciones principales (insert, search, height, minKey, maxKey, size, clear, toString)
- **`btree_operations.h`**: Contiene las implementaciones separadas de:
  - `rangeSearch()` y sus funciones auxiliares
  - `remove()` y todas sus funciones auxiliares (merge, borrow, fill, etc.)
  - `check_properties()` para validar las propiedades del B-Tree
  - `build_from_ordered_vector()` para construir un árbol desde un vector ordenado

### Archivos de Prueba

- **`main.cpp`**: Suite de pruebas principal (NO modificar según README)
- **`tester.h`**: Macros de testing (ASSERT)
- **`ejemplo_uso.cpp`**: Ejemplo de cómo usar las funciones separadas
- **`tests/`**: Carpeta con todos los archivos de test adicionales

## ¿Por qué están separadas estas funciones?

Las funciones `rangeSearch`, `remove`, `check_properties` y `build_from_ordered_vector` están implementadas en `btree_operations.h` por las siguientes razones:

1. **Organización**: Separa la lógica compleja de eliminación y validación del código base
2. **Modularidad**: Permite trabajar en estas funciones de manera independiente
3. **Claridad**: `btree.h` se mantiene más enfocado en las operaciones básicas del árbol
4. **Reutilización**: Las funciones auxiliares pueden ser accedidas mediante declaraciones `friend`

## Cómo Usar

Para usar el B-Tree, solo necesitas incluir `btree.h`:

```cpp
#include "btree.h"

int main() {
    // btree.h automáticamente incluye btree_operations.h
    BTree<int>* tree = new BTree<int>(3);
    
    tree->insert(10);
    tree->remove(10);  // Implementada en btree_operations.h
    
    vector<int> range = tree->rangeSearch(5, 15);  // Implementada en btree_operations.h
    
    bool isValid = tree->check_properties();  // Implementada en btree_operations.h
    
    delete tree;
    return 0;
}
```

## Compilación

```bash
# Compilar main.cpp
g++ -std=c++11 main.cpp -o main.exe

# Compilar ejemplo de uso
g++ -std=c++11 ejemplo_uso.cpp -o ejemplo_uso.exe

# Compilar cualquier test
g++ -std=c++11 tests/test_nombre.cpp -o tests/test_nombre.exe
```

## Funciones Implementadas

### En `btree.h`:
- `insert(key)` - Inserción con estrategia post-inserción
- `search(key)` - Búsqueda de elemento
- `height()` - Altura del árbol (leaf = 0)
- `minKey()` - Clave mínima
- `maxKey()` - Clave máxima
- `size()` - Número total de elementos
- `clear()` - Vaciar el árbol
- `toString(sep)` - Representación en string (inorder)

### En `btree_operations.h`:
- `remove(key)` - Eliminación de elemento con todas sus funciones auxiliares
- `rangeSearch(begin, end)` - Búsqueda de elementos en un rango
- `check_properties()` - Validación de propiedades del B-Tree
- `build_from_ordered_vector(vector, M)` - Construcción desde vector ordenado

## Notas Técnicas

- **Orden del árbol (M)**: Se soportan valores pares e impares (probado con M=3, 4, 5, 7)
- **Estrategia de inserción**: Post-inserción (insertar primero, dividir después si count == M)
- **Definición de altura**: Hojas tienen altura 0, árbol vacío tiene altura 0
- **Propiedades validadas**: 
  - Todas las hojas al mismo nivel
  - Número correcto de claves por nodo (mínimo/máximo)
  - Claves ordenadas en cada nodo
  - Número correcto de hijos en nodos internos
