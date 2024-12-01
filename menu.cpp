#include "menu.h"
#include <iostream>
#include <chrono>
#include <set>
#include <algorithm>
#include <sstream>
#include <Historial.h>
#include <conio.h>
#include <fstream>

#include "Movie.h"
using namespace std;

void GenerateSpaces(){
    for (int i = 0; i < 40; i++) cout << "\n";
}

void showSynopsis(string sinopsis) {
    GenerateSpaces();
    cout << "Sinopsis:\n" << sinopsis << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                 1.Dar Like            2.Agregar a Ver Mas Tarde                " << endl;
    cout << "         Presione una tecla diferente para poder volver a la pagina anterior    " << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

void showLikes(ifstream& file){
    string line, id;
    getline(file, line);
    cout<<"*** Lista de likes *** ";
    while (true) {
        getline(file, id);
        if(file.eof()) break;
        cout<<id<<endl;
    }
    cout<<endl;
}

template <typename... Vectors>
void AsignedMovies(const unordered_map<string, Movie*>& mapa,
                   TrieNode& trieTitle,
                   TrieNode& trieSynopsis,
                   TrieNode& trieTags,
                   chrono::duration<double> duration,
                   const Vectors&... vector_ids) {
    set<string> printed_titles;       // Evitar duplicados
    int count = 0;                   // Contador de películas mostradas
    bool exit_loop = false;          // Controlar la salida del bucle principal
    vector<Movie*> movies_5; // Almacena las películas actuales
    Historial historial;             // Objeto para manejar los estados
    int counter = 0;                 // Contador para gestionar estados del historial
    vector<string> vecPeliculasLikeadas; // Vector con ids de películas con like

    // Lambda para procesar y cargar las películas
    auto process_id = [&](const string& id) -> bool {
        auto it = mapa.find(id);
        if (it != mapa.end() && printed_titles.insert(it->second->getTitulo()).second) {
            Movie* movie = it->second;
            movies_5.emplace_back(movie);
            count++;
            // Cada vez que se llenen 5 películas, crea un nuevo estado
            if (count % 5 == 0) {
                Memento memento(movies_5);
                historial.agregarEstado(memento);
                movies_5.clear(); // Limpia para preparar el siguiente grupo
            }
        }
        return true;
    };

    // Procesar todos los vectores
    ([&]() {
            for (const auto& id : vector_ids) {
                if (exit_loop) break; // Romper el bucle si se activa `exit_loop`
                process_id(id);      // Procesar cada ID individual
            }
    }(),...); // Expansión del pack para aplicar a cada vector

    //Añadirlas si es que no llegan a 5
    if (!movies_5.empty()) {
        Memento memento(movies_5);
        historial.agregarEstado(memento);
    }

    // Bucle principal para interactuar con el usuario
    int option = 0;
    while (!exit_loop) {
        GenerateSpaces();
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "                                     Peliculas disponibles:\n";
        cout << "------------------------------------------------------------------------------------------------------\n";

        // Mostrar el estado actual del historial
        historial.orderEstados();
        historial.printEstadoActual(counter);

        // Mostrar opciones
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "                                      Seleccione una opcion \n";
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "1 - 5.             Ver sinopsis de la pelicula correspondiente.\n";
        cout << "6.                 Ver las siguientes 5 peliculas.\n";
        cout << "7.                 Regresar al menu principal.\n";
        cout << "8.                 Volver al grupo anterior de peliculas.\n";
        cout << "------------------------------------------------------------------------------------------------------\n";
        cin >> option;
        if (option >= 1 && option <= 5) {
            // Mostrar sinopsis de la película seleccionada
            auto& estados = historial.getter_estados();
            if (!estados.empty() && counter < static_cast<int>(estados.size())) {
                Memento& estado_actual = estados[counter];
                estado_actual.order(); //ordenamos la lista de movies de acuerdo al peso
                showSynopsis( estado_actual.getSynopsis(option - 1));
                int e;
                cin >> e;
                while(e == 1 || e==2){
                    if(e==1){
                        cout<<estado_actual.printMovieTitle(option-1);
                        estado_actual.like(option-1);

                        //Primero, leer archivo con peliculas likeadas:

                        ifstream archLikesLeer("../listaLikes.txt", ios::in);
                        if (!archLikesLeer.is_open()) {
                            // Si el archivo de ganadores No existe:
                            cout << "No se pudo encontrar un registro de ganadores existente" << endl;
                            cout << "Creando nueva lista de ganadores..." << endl;

                            // Dar valor inicial al vector agregando la nueva pelicula likeada:
                            vecPeliculasLikeadas.emplace_back(ID);

                        } else { // Si el archivo de ganadores Sí existe:
                            //Detectar si existe el ganador actual en el archivo ganadores. Si está, actualizar su contador. Si no, lo añade.

                            //Primero, llenar los vectores con las peliculas likeadas existentes:
                            string linea, id;
                            bool peliEncontrada = false;
                            getline(archLikesLeer, linea);
                            while (true) {
                                archLikesLeer>>id;
                                if(archLikesLeer.eof()) break;
                                vecPeliculasLikeadas.emplace_back(id);
                            }

                            // Ahora, recorrer el vector de nombre buscando al id de la pelicula:
                            for (int i = 0; i < vecPeliculasLikeadas.size(); i++) {
                                if (ID == vecPeliculasLikeadas[i]) {
                                    peliEncontrada = true; // Si lo encuentra,
                                    break; // deja de buscar
                                }
                            }
                            // Si no lo encuentra, lo añade
                            if (!peliEncontrada) {
                                vecPeliculasLikeadas.emplace_back(ID);
                            }

                            // Ya está el vector actualizadas con las películas likeadas.
                            // Ahora, a escribirlos en el archivo listaLikes.txt

                        }
                        archLikesLeer.close(); // Cerramos el archivo (es buena práctica)

                        // Ahora, a escribir el archivo (siempre se sobreescribe)
                        ofstream archLikesEscribir("../listaLikes.txt", ios::out);

                        archLikesEscribir<<"Lista de likes: "<<endl;
                        for (int i = 0; i < vecPeliculasLikeadas.size(); i++) {
                            archLikesEscribir << vecPeliculasLikeadas[i]<<endl;
                        }

                        archLikesEscribir.close();
                    }
                    else if(e==2)estado_actual.later(option-1);
                    showSynopsis(estado_actual.getSynopsis(option - 1));
                    cin >> e;
                }
            } else {
                cout << "No hay sinopsis disponible.\n";
            }
        } else if (option == 6) {
            // Ver las siguientes 5 películas
            if (counter < static_cast<int>(historial.getter_estados().size()) - 1) {
                counter++;
            } else {
                cout << "No hay más películas para mostrar.\n";
            }
        } else if (option == 8) {
            // Retroceder al grupo anterior
            if (counter > 0) {
                counter--;
            } else {
                cout << "No hay grupos anteriores.\n";
            }
        } else if (option == 7) {
            // Salir al menú principal
            exit_loop = true;
            showMenu(trieTitle, trieSynopsis, trieTags, mapa, duration);
        } else {
            cout << "Opcion inválida. Intente nuevamente.\n";
        }
    }
}

void showMenu( TrieNode& trieTitle, TrieNode& trieSynopsis, TrieNode& trieTags,
               const unordered_map<string, Movie*>& mapa_ids,
               chrono::duration<double> duration) {
    int option = 0;
    vector<int> options = {1, 2, 3, 4, 5};
    auto it = find(options.begin(), options.end(), option);

    while (it == options.end()) {
        cout
                << "########################################################################################################\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "#                                       WELCOME TO MOVIE STREAMING                                     #\n";
        cout
                << "#                                           ~ Your Movie World ~                                       #\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "########################################################################################################\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "#      ________                   ________                 ________             ________               #\n";
        cout
                << "#     |        |                 |        |               |        |            |       |              #\n";
        cout
                << "#     |  MOVIE |                 | MOVIE  |               | MOVIE  |            | MOVIE |              #\n";
        cout
                << "#     | POSTER |                 | POSTER |               | POSTER |            | POSTER|              #\n";
        cout
                << "#     |_______ |                 |________|               |________|            |_______|              #\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "#                  Trending Now:         Watch Again:         Recommended for You:                     #\n";
        cout
                << "#                   ______________          ______________          ______________                     #\n";
        cout
                << "#                  |              |        |              |        |              |                    #\n";
        cout
                << "#                  |   MOVIE A    |        |   MOVIE B    |        |   MOVIE C    |                    #\n";
        cout
                << "#                  |______________|        |______________|        |______________|                    #\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "# ---------------------------------------------------------------------------------------------------- #\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "#                         [1] Search Movies  |  [2] Browse by Tags  |  [3] Watch Later                 #\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "#                                  [4] View Liked Movies   |   [5] Exit                                #\n";
        cout
                << "#                                                                                                      #\n";
        cout
                << "########################################################################################################\n";
        cout
                << "#                                 Select an option to continue...                                      #\n";
        cout
                << "########################################################################################################\n\n";
        cout << "TIEMPO DE DURACION: " << duration.count() << " segundos\n";
        cin >> option;

        if (option == 1) {
            string busqueda;
            GenerateSpaces();
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                           Ingrese el nombre de la pelicula                     " << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            cin.ignore();
            getline(cin, busqueda);
            istringstream iss(busqueda);
            string word;
            GenerateSpaces();
            while (iss >> word) {
                vector<string> results = trieTitle.searchByPrefix(word);
                vector<string> results2 = trieSynopsis.searchByPrefix(word);
                AsignedMovies(mapa_ids, trieTitle, trieSynopsis, trieTags, duration, results);
            }
        } else if (option == 2) {
            string tag;
            GenerateSpaces();
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                             Ingrese el tag de la pelicula                      " << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            cin.ignore();
            getline(cin, tag);
            vector<string> results = trieTags.searchByPrefix(tag);
            AsignedMovies(mapa_ids, trieTitle, trieSynopsis, trieTags, duration, results);
        } else if (option == 4){
                ifstream archLikesLeer("../listaLikes.txt",ios::in);
                if (!archLikesLeer.is_open()) {
                    cout << "No se han registrado likes." << endl;
                }else{
                    showLikes(archLikesLeer);
                }
        } else if (option == 5) {
            GenerateSpaces();
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                Gracias por usar Movie Streaming. ¡Hasta luego!" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            break;
        } else {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                           Opción no válida. Intente nuevamente.\n";
            cout << "--------------------------------------------------------------------------------" << endl;
        }
        it = find(options.begin(), options.end(), option);
    }
}
