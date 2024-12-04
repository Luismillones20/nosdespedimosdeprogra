## Explicación de los códigos
### Funciones básicas de trie:
Para el código se usaron las siguientes librerías en C++:
```cpp
#include <unordered_map> 
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <algorithm>
```
Existen 3 árboles: TrieTitle, TrieSynopsis, TrieTags lo que hace cada uno es ingresar uno a uno los caracteres legibles por ASCII (no chinos, ni japoneses) de cada una de las partes correspondientes.
#### findWordsWithPrefix:
Lo que busca es encontrar las palabras por prefijo.
- #### Insert():
Lo que hace es insertar caracter a caracter en un árbol, ya sea en el TrieTitle, TrieTags o el TrieSynopsis

### Funciones básicas de menu.cpp:
- #### AsignedMovies():
- #### ShowMovies(): 
### Funciones básicas de proyecto.cpp: 

- #### `GetCorrectGetLines()`
  Obtiene correctamente las líneas, ya que, el `getline()` obtiene todo un string hasta la primera `"`, pero dado que cuando en el csv hay 2 `"` esto genera problemas. Por lo tanto, lo que hace esta función es asegurarse que esto no ocasione problemas y se obtenga correctamente la sinopsis.
- #### `InsertWordByWordToTheTrie()`
  Inserta palabra a palabra en el árbol Trie, ya sea TrieTitle, TrieSynopsis, TrieTags, ya que el `getline` obtiene un string gigante con toda el título, en el caso de TrieTitle, esta función hace que se eliminen los espacio y se ingresen por palabras a otra función.
- #### `insert()`
  Consecuentemente, después de la función `InsertWordByWordToTheTrie()` esta función obtiene las palabras obtenida por este y las divide en caracteres, los que finalmente serán insertados en árbol.
- #### `AsignedMovies()`
  Esta función enseña las películas que salen en la terminal, asegurándose de que sean solo 5 películas y también obtener las sinopsis.
- #### `AsignedMovies()`




## Link Video:
  https://drive.google.com/drive/folders/1rlhTjliY6vIhgXSjpQ3aeGyPw4a1E13B?usp=sharing

