[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/INK5rVnQ)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=20462810&assignment_repo_type=AssignmentRepo)

# Tabla Hash con Chaining

## Capturas de pantalla

![p1](https://raw.githubusercontent.com/utec-cs-aed-2025-1/ejercicios-01-hash-table-Grazen0/refs/heads/main/screenshot_p1.png)

![p2](https://raw.githubusercontent.com/utec-cs-aed-2025-1/ejercicios-01-hash-table-Grazen0/refs/heads/main/screenshot_p2.png)

![p3](https://raw.githubusercontent.com/utec-cs-aed-2025-1/ejercicios-01-hash-table-Grazen0/refs/heads/main/screenshot_p3.png)

## Descripción

Implementar una tabla hash que utiliza **chaining** para resolver colisiones mediante listas enlazadas manuales. La clase `ChainHash` utiliza nodos para crear cadenas en cada bucket.

## Estructura de Datos

- **ChainHashNode**: Estructura que contiene `key`, `value`, `hashcode` y puntero `next`
- **ChainHashListIterator**: Iterador para recorrer los elementos de un bucket específico
- **Array de punteros**: Cada posición apunta al primer nodo de la cadena del bucket

## Métodos Ya Implementados

- `get(key)` - Obtener valor por clave
- `size()` - Número total de elementos
- `bucket_count()` - Capacidad total del array
- `bucket_size(index)` - Elementos en un bucket específico

## Métodos a Implementar

### 1. `void set(TK key, TV value)`

Insertar/actualizar elemento. Manejar rehashing si es necesario.

### 2. `bool remove(TK key)`

Eliminar elemento por clave. Retornar true si se eliminó.

### 3. `bool contains(TK key)`

Verificar si existe una clave en la tabla hash.

### 4. `ChainHashListIterator begin(int index)`

Retornar iterador al inicio del bucket especificado.

### 5. `ChainHashListIterator end(int index)`

Retornar iterador al final del bucket especificado.

### 6. `void rehashing()`

Redimensionar la tabla hash cuando sea necesario.

### 7. `~ChainHash()` (Destructor)

Liberar toda la memoria utilizada.

## Variables Importantes

- `Node** array` - Array de punteros a nodos (buckets)
- `nsize` - Total de elementos insertados
- `capacity` - Tamaño del array de buckets
- `usedBuckets` - Buckets que tienen al menos un elemento
- `bucket_sizes[]` - Elementos por bucket

## Constantes del Sistema

- `maxColision = 3` - Máximo elementos por bucket antes de rehashing
- `maxFillFactor = 0.8` - Factor de carga máximo (buckets ocupados / total buckets)

## Notas Importantes

- **Factor de Carga**: Se calcula como proporción de **buckets ocupados**, no total de elementos
- **Rehashing**: Se activa por factor de carga excesivo O colisiones excesivas
- **Iteradores**: `ChainHashListIterator` solo recorre un bucket específico
- **Contadores**: Mantener sincronizados `nsize`, `bucket_sizes` y `usedBuckets`

## Resolver ejercicios:

- **P1:** Cargar datos desde CSV y mostrar la distribución en buckets. Implementar los iteradores `begin()` y `end()` para recorrer elementos de cada bucket específico.
- **P2:** Implementar algoritmo "Bag of Words" usando tabla hash. Crear un diccionario que mapee cada palabra a los índices de documentos donde aparece.
- **P3:** Resolver el problema de intersección de listas enlazadas usando tabla hash. Implementar `connectLists()` para crear intersecciones y `getIntersectionNode()` para encontrarlas eficientemente.

## Compilación y Ejecución

```bash
g++ -o p1 p1.cpp
./p1
```
