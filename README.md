# Programación III: Proyecto Final (2024-2)
- Profesor: José Chávez
- Curso: Programación III
- Ciclo 2024-2 - Diciembre, 2024
- Universidad de Ingeniería y Tecnología (UTEC)
## Integrantes
- ##### Sebastián Hernández Miñano - 202320043
- ##### Luis Millones Carrasco - 202320115
- ##### David Huette Ospino - 202320048
- ##### Andrés Torres Ramos - 202320163

## Explicación de los códigos

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
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
```

A continuación se detalla una descripción de las funciones implementadas en cada archivo del programa. Representan el funcionamiento entero, y se seguirá el flujo de archivos que se recorre desde la ejecución del programa hasta su uso en directo.

### Clases implementadas:
- #### `Movie` en `Movie.h`:
  #### Propósito General:
        La clase Movie encapsula los datos y comportamientos relacionados con una película, como asignar puntuaciones, marcarla para acciones específicas y notificar a otros componentes del sistema sobre dichos cambios. Los métodos permiten una fácil interacción y personalización de las películas dentro del programa.
  #### Atributos:
        string id;          // Identificador único de la película
        string name;        // Título de la película 
        string sinopsis;    // Sinopsis de la películas
        int peso = 0;       // Puntuación de la película (para ordenarla)
        bool like = false;  // Indica si la película tiene un "Like"
        bool forLater = false; // Indica si la película está marcada como "Ver Más Tarde"
        vector<Observer*> observers; // Lista de observadores

  #### Métodos implementados:
  - `Movie()`: Constructor por defecto
  - `Movie(string id_, string name_, string sino_)`: Constructor que inicializa los atributos id, name y sinopsis.

  - `notifyObservers()`: Notifica a todos los observadores registrados sobre una acción específica realizada en la película (por ejemplo, "Like" o "Add Later").
  - `~Movie()`: Destructor por defecto. Libera los recursos de la clase.
  #### Métodos originales con notificación:
  - `givelike()`: Marca la película como "Like". Si aún no estaba marcada, incrementa el peso en 5 y notifica a los observadores.

  - `addLater()`: Marca la película como "Ver Más Tarde". Si aún no estaba marcada, incrementa el peso en 3 y notifica a los observadores.

  - `verifyPeso()`: Devuelve el peso actual de la película.

  - `hasLater()`: Devuelve true si la película está marcada como "Ver Más Tarde".

  - `haslike()`: Devuelve true si la película tiene un Like.

  #### Getters and Setters:
  - `getId()`
  - `getTitulo()`
  - `getsinopsis()`
  - `getPeso() `
  - `setPeso()`
  - `setLike()`
  - `setForLater()`

- #### `FavoritesManager : public Observer` en `Observer.h`

  #### Propósito General:
  Gestionar las listas de películas con "Like" y "Ver Más Tarde". Permite agregar y consultar películas en estas listas y guardar y cargar las listas desde archivos. Además, se integra con otros patrones de diseño como Observer (para recibir notificaciones de cambios) y Decorator (para personalizar la presentación de las listas).

  #### Atributos:
        unordered_map<string, Movie*> movieMap;          // Mapa que asocia los IDs de las películas con sus objetos correspondientes
        set<string> favorites;        // Almacena los IDs de las películas que tienen "Like" 
        set<string> watchLater;    // lmacena los IDs de las películas marcadas como "Ver Más Tarde"
        const string likesFile;       // Ruta del archivo donde se guarda la lista de películas con "Like"
        const string watchLaterFile = false;  // Ruta del archivo donde se guarda la lista de películas "Ver Más Tarde"

  #### Métodos de gestión de listas:
  - `FavoritesManager(unordered_map<string, Movie*> movieMap_)`: Constructor que inicializa el mapa de películas (movieMap) y carga las listas de "Likes" y "Ver Más Tarde" desde los archivos
  - `isLiked()`: Verifica si una película está en la lista de Likes.
  - `isWatchLater()`: Verifica si una película está en la lista de "Ver Más Tarde".
  - `likeMovie()`: Agrega una película a la lista de Likes.
  - `addToWatchLater()`: Agrega una película a la lista de "Ver Más Tarde"
  #### Métodos de carga y guardado en archivos:
  - `loadLikes()`: Lee el archivo de "Likes" y carga los IDs de las películas al conjunto favorites.
  - `loadWatchLater()`: Lee el archivo de "Ver Más Tarde" y carga los IDs de las películas al conjunto watchLater.
  - `saveLikes()`: Guarda los IDs y títulos de las películas con "Like" en el archivo likesFile
  - `saveWatchLater()`: Guarda los IDs y títulos de las películas marcadas como "Ver Más Tarde" en el archivo watchLaterFile
  #### Métodos para mostrar listas:
  - `showFavorites()`: Muestra en consola la lista de películas con Like.
  - `showForLater()()`: Muestra en consola la lista de películas marcadas como "Ver Más Tarde".
  #### Métodos relacionados con el patrón Decorator:

  - `showLikesDecorator()`: Utiliza un decorador para mostrar la lista de películas con "Like".
  - `showMasTardeDecorator()`: Utiliza un decorador para mostrar la lista de películas "Ver Más Tarde".



- #### `Decorator` en `Decorator.h`:

  #### Propósito General:
  Es la interfaz base para todos los decoradores de estilos. Define el contrato para los métodos de visualización.

  #### Métodos implementados:
  - `virtual display()`: Método virtual puro, recibe un vector de ids para mostrar, y un mapa que relaciona IDs con Movies.
  - `virtual ~Decorator()`: Destructor virtual para asegurar una correcta liberación de recursos en clases derivadas.

- #### `LikeDecorator : public Decorator` en `Decorator.h`:

  #### Propósito General:
  Personaliza la visualización de la lista de películas marcadas con "Like".

  #### Métodos implementados:
  - `display()`: Implementación del método virtual. Muestra en consola una lista de las películas con Like. Imprime un encabezado con el título 🌟 "Películas con Like"

- #### `VerMasTardeDecorator : public Decorator` en `Decorator.h`:

  #### Propósito General:
  Personaliza la visualización de la lista de películas marcadas como "Ver Más Tarde".

  #### Métodos implementados:
  - `display()`: Implementación del método virtual. Muestra en consola una lista de las películas con Like. Imprime un encabezado con el título ⏳ "Ver Más Tarde"


- #### `Memento` en `Memento.h`

  #### Propósito General:
  Representa un estado de la aplicación, que incluye un grupo de películas (generalmente 5). Implementa el patrón Memento, que permite capturar y restaurar el estado de lo mostrado en pantalla.

  #### Atributos:
        vector<Movie*> current5movies; // Contiene un grupo de punteros a películas que representan el estado actual

  #### Métodos implementados:
  - `Memento(vector<Movie*>& movies5)`: Constructor que inicializa el estado con un vector de películas (movies5).
  - `~Memento()`: Destructor por defecto para liberar recursos asociados.
  - `order()`: Ordena las películas almacenadas en current5movies de mayor a menor según su peso (peso).
  - `getState()`: Genera un resumen en formato texto del estado actual y ordena las películas.
  - `like()`: Marca como "Like" la película en la posición 'num' dentro de current5movies.
  - `later()`: Marca como "Ver Más Tarde" la película en la posición 'num'.
  - `getSynopsis()`: Devuelve la sinopsis de la película en la posición num e indica si tiene Like o está en "Ver Más Tarde"
  #### Getters:
  - `getMovieId()`
  - `getMovieTitle()`



- #### `Historial` en `Historial.h`

  #### Propósito General:
  Actúa como un contenedor de estados de películas. Se utiliza para gestionar los cambios en las películas mostradas (como "Likes" o "Ver Más Tarde") y facilita la navegación entre diferentes estados.

  #### Atributos:
        vector<Memento> estados; // Almacena un historial de estados representados por objetos de tipo Memento

  #### Métodos implementados:
  - `agregarEstado(unordered_map<string, Movie*> movieMap_)`: Añade un nuevo estado (Memento) al historial.
  - `clearEstados()`: Limpia todos los estados almacenados en el historial.
  - `printEstadoActual()`: mprime en consola el estado actual según el índice counter. Llama a `getState()` del `Memento` correspondiente.
  #### Métodos de carga y guardado en archivos:
  - `getter_estados()`: Devuelve una referencia al vector estados.

- #### `TrieNode` en `Trie.h`

  #### Propósito General:
  Actúa como un contenedor de estados de películas. Se utiliza para gestionar los cambios en las películas mostradas (como "Likes" o "Ver Más Tarde") y facilita la navegación entre diferentes estados.

  #### Atributos:
        unordered_set<int> movieIds; // IDs de películas donde aparece tal palabra **Nota en Anexo (1)*
        string ID;                   // Id de la película, el identificador asociado al nodo
        char data;                   // Almacena el carácter del nodo (solo para fines de depuración o impresión)
        TrieNode* children[N];       // Array de punteros que representa los hijos del nodo actual. Cada índice corresponde a un carácter (basado en un conjunto de N = 256 caracteres posibles).
        bool is_leaf;                // Indica si el nodo actual es una hoja (es decir, si representa el final de una palabra completa).

  #### Métodos implementados:
  Solo se implementaron las interfaces de los métodos:

  - `TrieNode()`
  - `TrieNode(char data)`
  - `searchByPrefix()`
  - `findWordsWithPrefix()`


### Funciones de proyecto_final.cpp:

- #### `GetCorrectGetLines()`
  Usado para la lectura de la sinopsis, obtiene correctamente las líneas dado que cuando en el archivo .csv hay ocasiones donde dentro de la sinopsis, se divisan dos comillas (`"`) seguidas para indicar que se tratan de comillas normales, lo cual genera problemas al usar la función `getline()`. Por lo tanto, con está función se toma en cuenta esos casos con condicionales que evaluán si estas comillas se tratan de comillas de delimitación o son parte de la sinopsis.

- #### `GetTitleAndTag()`
  Usado para la lectura del título y los tags de la película, tal como en el caso anterior toma en cuenta casos que dificultan el uso de la función `getline()` para obtener correctamente estos datos. En este caso, existen situaciones donde el título contiene comillas entre las palabras, o casos donde aparece más de un tag en una película, por lo que los tags se dividen por comas, todos dentro de un par de comillas. La función `GetTitleAndTag()` contempla estas situaciones.

- #### `InsertWordByWordToTheTrie()`
  Inserta palabra a palabra en el árbol Trie, ya sea TrieTitle, TrieSynopsis o TrieTags. Se da el caso que las funciones de obtención de los títulos, sinopsis y tags devuelven todo el contenido leído como un solo string gigante con todas las palabras. Es por ello que esta función hace que se eliminen los espacios entre las palabras, y estas se vayan ingresando una por una la función `insert()`.

- #### `insert()`
  Consecuentemente, después de la función `InsertWordByWordToTheTrie()`, esta función obtiene las palabras obtenidas por este y las divide en caracteres, los cuales finalmente serán insertados en árbol. Cada caracter pasa por un proceso de búsqueda en los árboles existentes, lo cual evita que se generen nuevos nodos innecesariamente, y estos se vayan generando de manera correcta siguiendo el orden de caracteres para cada palabra dada.

### Funciones de Trie.cpp:
Para implementar el guardado de los, se implementaron 3 árboles:
- TrieTitle - Árbol de Títulos (titles)
- TrieSynopsis - Árbol de Sinopsis (plot_synopsis)
- TrieTags - Árbol de Tags (tags)
  Lo que hacen es ingresar uno a uno los caracteres legibles por ASCII (no chinos, ni japoneses) de cada una de las secciones correspondientes.
  A continuación se detallen las funciones implementadas en el archivo:

- #### `searchByPrefix()`
Función auxiliar para encontrar palabras que concuerden con lo ingresado, inicializa un vector de ids para guardar los resultados que se encuentren. Llama a la función `findWordsWithPrefix()`, el cual recibe la cadena ingresada y el vector de ids para actualizarlo.

- #### `findWordsWithPrefix()`
Encuentra las palabras por prefijo ingresado. Va buscando una sucesión de nodos que cumpla con el orden de caracteres ingresado. Posteriormente, llama a la función `findAllWords()`, la cual recibe el árbol que tiene la sucesión de caracteres pedida.

- #### `findAllWords()`
Función auxiliar para encontrar todas las palabras a partir de un nodo dado. Busca entre los hijos de la palabra ingresada si hay palabras más extensas que se formen a partir de la palabra encontrada (por ejemplo "carr" -> "carro" -> "carros", "carroza"), y va actualizando el vector de IDs con IDs de películas donde se encuentren esas palabras.

- #### `insert()`
Lo que hace es insertar caracter a caracter en un árbol, ya sea en el TrieTitle, TrieTags o el TrieSynopsis

- #### `TrieNode()`
Constructores de la clase TrieNode.

### Funciones de menu.cpp:

- #### `showMenu()`
  Muestra la interfaz inicial y principal del programa, donde se muestran las opciones que el usuario tiene para empezar con el flujo de la misma.
  Primero, crea las instancias para los patrones Observer y Decorator.
  Posteriormente, imprime la interfaz inicial, y permite elegir entre 5 opciones:
  - Opcion `1`: Búsqueda de películas por nombre y sinopsis. Permite ingresar una cadena de texto, ya sea una porción de palabra, una palabra entera o un conjunto de palabras, para ser buscada tanto en el árbol de títulos como de sinopsis.
  - Opcion `2`: Búsqueda de películas por tags. Se ingresa una porcion o palabra entera correspondiente a un tag y busca las palículas correspondientes a los Tags encontrados.
    **Nota en Anexo (1)*
  - Opcion `3`: Despliega la lista de Ver Más Tarde. Usa el patrón Decorator para agregarle emojis a la impresión. Además, usa la función `callMenuAgain()`, diseñada para invocar nuevamente el menú principal de la aplicación de películas.
  - Opción `4`: Despliega la lista de Likes. También usa el patrón Decorator y la función `callMenuAgain()`.
  - Opción `5`: Termina el programa.

- #### `AsignedMovies()`
  Esta función realiza una serie de tareas que se encargan del procesamiento de los IDs obtenidos, correspondientes a la búsqueda ingresada, la correcta paginación de las películas correspondientes y la impresión de estas. Usa tanto funciones definidas dentro de sí misma como métodos de clases que también implementa en su código. Debido al tamaño, se detallan a continuación solo las funciones que corresponden exclusivamente a su propiedad (los métodos usados se detallan en la sección de sus respectivas clases):

  - ##### `process_id()`
    Función lambda (llamada por otra función lambda que envía los ids del vector de Ids de películas obtenidas) que recibe ids para encontrar las películas correspondientes (en formato de objetos de la clase Movie). Va guardando lo obtenido en un vector que posteriormente usará la función `actualizarHistorial()` para manejar la paginación y correcta muestra de las películas resultado.

  - ##### `actualizarHistorial()`
    Función lambda que recibe el vector de películas de `process_id()` para luego agruparlas de a 5 y mostrarlas correctamente en la consola. Antes de ello, las ordena en tiempo real mediante el peso que obtienen al recibir un Like o ser guardadas en la lista de Ver más tarde. Dentro de esta función también se encuentra implementada el guardado del historial del conjunto de películas mostradas mediante el patrón de diseño `Memento` en conjunto con las clase `Historial`.

    Después de esto, se muestra la interfaz donde se muestran las películas encontradas. Se ayuda de `GenerateSpaces()` para generar el espacio de separación entre las diferentes interfaces. Se permite elegir entre ciertas opciones para navegar o interactuar con las películas en pantalla. Se brindas distintas opciones:

    - Opciones `1-5`: Permiten visualizar la sinopsis de la película en cuestión ejecutando `showSynopsis()`, desplegando el texto y los indicadores ("Sí" o "No") de si está marcada con Like o Ver Más Tarde, permitiendo alterar estos estados en este momento.
    - Opción `6`: Ver las siguientes 5 películas
    - Opción `7`: Regresar al menú principal
    - Opción `8`: Volver al grupo anterior de películas. Ejecuta la función `showMenu()`.


### Funciones de FuncionesMenu.h:
Implementación de las funciones usadas a lo largo de `menu.cpp`:
- #### `GenerateSpaces()`
- #### `showSynopsis()`
- #### `callMenuAgain()`

### Funciones de menu.h:
Interfaces de las siguientes funciones usadas en `menu.cpp`:
- #### `showMenu()`
- #### `AsignedMovies()`

### Funciones de Trie.h:
Interfaces de los siguientes métodos usados en `Trie.cpp`:
- #### `TrieNode()`
- #### `TrieNode(char data)`
- #### `searchByPrefix()`
- #### `findWordsWithPrefix()`

### Funciones no usadas:
Las siguientes funciones tenían un propósito en los avances preliminares, pero fueron dejadas de lado en la solución final:
- #### `print()` en `Trie.cpp`
- #### `search()` en `Trie.cpp`
- #### `delete_word()` en `Trie.cpp`
- #### `find_longest_prefix()` en `Trie.cpp`
- #### `is_leaf_node()` en `Trie.cpp`
- #### `parallelFindWordsWithPrefix()` en `Trie.cpp`
- #### `addObserver()` en `Movie.h`
- #### `orderEstados()` en `Historial.h`
- #### `getEstadoAnterior()` en `Historial.h`
- #### `threadSafeAddToHistorial()` en `Historial.h`



## Link del Video (Presentación Grupal de Explicación del Código):
https://drive.google.com/drive/folders/1rlhTjliY6vIhgXSjpQ3aeGyPw4a1E13B?usp=sharing


### Anexo
- #### 1 :
  El problema que se tuvo con el agrupamiento de IDs correspondía a solucionar el hecho de que hubieran películas con mismas palabras o partes de palabras ya sea en los títulos o en las sinopsis. Lo que se buscó fue agregar un conjunto (ya sea un vector, set, arreglo) de IDs al final de cada palabra para que se indicara en qué películas se encontraba la palabra buscada. Sin embargo, al intentar implementar la función se presentó un `SIGNAL ERROR` que resultaba al intentar ingresar los IDs uno por uno al vector de IDs presente en cada caracter de final de palabra (en el nodo). Se intentó rehacer el código de implementación, pero el error persistió y esto imposibilitó que se pudiese mostrar más de una película asociada a una palabra (tanto al buscar películas por título/sinopsis como al buscar por tags).

  Este error corresponde a cuando se lee el archivo .csv y se guardan las palabras. El ID se actualiza cuando se lee una palabra igual a la que ya tenía un ID. Por esta razón, al buscar "Shrek", solo sale "Shrek 2", el cual está más al final en el archivo. Lo mismo ocurre al buscar por tags: si se ingresa "horr" o "horror", solo se muestra la última película que tuvo entre el tag "horror".



# Referencias

1. *Sorber, J. (2023).*  
   C++ The Trie Data Structure (Prefix Tree) [Video]. YouTube.  
   [https://www.youtube.com/watch?v=3CbFFVHQrk4&t=772s&ab_channel=JacobSorber](https://www.youtube.com/watch?v=3CbFFVHQrk4&t=772s&ab_channel=JacobSorber)

2. *Refactoring Guru. (2024).*  
   Decorator: Patrón de diseño (secciones "Estructura" y "Pseudocódigo"). Refactoring Guru.  
   [https://refactoring.guru/es/design-patterns/decorator](https://refactoring.guru/es/design-patterns/decorator)

3. *DigitalOcean. (s.f.).*  
   Trie Data Structure in C++ (secciones "Implementing a Trie Data Structure in C/C++" y "Time Complexity for the above Procedures"). DigitalOcean.  
   [https://www.digitalocean.com/community/tutorials/trie-data-structure-in-c-plus-plus](https://www.digitalocean.com/community/tutorials/trie-data-structure-in-c-plus-plus)

4. *Refactoring Guru. (2024).*  
   Singleton: Patrón de diseño (secciones "Pseudocódigo" y "Esquema"). Refactoring Guru.  
   [https://refactoring.guru/es/design-patterns/singleton](https://refactoring.guru/es/design-patterns/singleton)

5. *Refactoring Guru. (2024).*  
   Memento: Patrón de diseño (secciones "Esquema" y "Pseudocódigo"). Refactoring Guru.  
   [https://refactoring.guru/es/design-patterns/memento](https://refactoring.guru/es/design-patterns/memento)

6. *Refactoring Guru. (2024).*  
   Observer: Patrón de diseño (secciones "Esquema" y "Pseudocódigo"). Refactoring Guru.  
   [https://refactoring.guru/es/design-patterns/observer](https://refactoring.guru/es/design-patterns/observer)

7. *Cplusplus.com. (s.f.).*  
   std::find (secciones "Find value in range" y "Parameters"). Cplusplus.com.  
   [https://cplusplus.com/reference/algorithm/find/](https://cplusplus.com/reference/algorithm/find/)

8. *GeeksforGeeks. (s.f.).*  
   map::find function in C++ STL (secciones "Syntax" y "Example of map::find()"). GeeksforGeeks.  
   [https://www.geeksforgeeks.org/map-find-function-in-c-stl/](https://www.geeksforgeeks.org/map-find-function-in-c-stl/)
   }