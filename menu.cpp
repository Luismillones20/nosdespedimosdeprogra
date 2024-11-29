#include "menu.h"
#include <iostream>
#include <chrono>
#include <set>
#include <algorithm>
#include <sstream>
using namespace std;

void GenerateSpaces(){
    for (int i = 0; i < 40; i++) cout << "\n";
}
template <typename... Vectors>
void AsignedMovies(const unordered_map<string, pair<string, string>>& mapa,
                   TrieNode& trieTitle,
                   TrieNode& trieSynopsis,
                   TrieNode& trieTags,
                   chrono::duration<double> duration,
                   const Vectors&... vector_ids){
    set<string> printed_titles;       // Evitar duplicados
    vector<pair<string, string>> current_movies; // Vector de películas actuales (título, sinopsis)
    int count = 0;                   // Contador de películas mostradas
    bool exit_loop = false;          // Controlar la salida del bucle principal

    // Lambda para procesar y cargar las películas
    auto process_id = [&](const string& id) -> bool {
        auto it = mapa.find(id);
        if (it != mapa.end() && printed_titles.insert(it->second.first).second) {
            current_movies.emplace_back(it->second.first, it->second.second);
            count++;
        }
        return true;
    };

    // Procesar todos los vectores
    ([&]() {
        for (const auto& id : vector_ids) {
            if (exit_loop) break;
            process_id(id);
        }
    }(), ...);

    // Bucle principal para interactuar con el usuario
    int option = 0;
    size_t start_index = 0; // Índice de inicio para mostrar películas

    while (!exit_loop) {
        GenerateSpaces();
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "                                     Peliculas disponibles:\n";
        cout << "------------------------------------------------------------------------------------------------------\n";

        for (size_t i = start_index; i < start_index + 5 && i < current_movies.size(); ++i) {  // Mostrar las siguientes 5 películas
            cout << i + 1 << ". " << current_movies[i].first << endl;
        }
        if (count == 5){
            count = 0;
            current_movies.empty();
        }
        // Mostrar opciones
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "                                      Seleccione una opcion \n";
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "1 - 5.             Ver sinopsis de la pelicula correspondiente.\n";
        cout << "6.                 Ver las siguientes 5 peliculas.\n";
        cout << "7.                 Regresar al menu principal.\n";
        cout << "------------------------------------------------------------------------------------------------------\n";
        cin >> option;

        if (option >= 1 && option <= 5) {
            size_t index = start_index + option - 1;
            if (index < current_movies.size()) {
                GenerateSpaces();
                cout << "Sinopsis de \"" << current_movies[index].first << "\":\n";
                cout << current_movies[index].second << endl;
                char espacio;
                cout << "-----------------------------------------------------------------------------------------------\n";
                cout << "                         Presione espacio para volver a las peliculas                              \n";
                cout << "-----------------------------------------------------------------------------------------------\n";
                cin >> espacio;
                GenerateSpaces();
            } else {
                cout << "Opcion inválida.\n";
            }
        } else if (option == 6) {
            // Avanzar al siguiente lote de 5 películas
            if (start_index + 5 < current_movies.size()) {
                start_index += 5;
            } else {
                cout << "No hay mas peliculas para mostrar.\n";
            }
        } else if (option == 8) {
            // Salir al menú principal
            exit_loop = true;
            showMenu(trieTitle, trieSynopsis, trieTags, mapa, duration);
        } else {
            cout << "Opcion inválida. Intente nuevamente.\n";
        }
    }
}


void showMenu( TrieNode& trieTitle, TrieNode& trieSynopsis, TrieNode& trieTags,
               const unordered_map<string, pair<string, string>>& mapa_ids,
               chrono::duration<double> duration) {
    int option = 0;
    vector<int> options = {1, 2, 3, 4, 5};
    auto it = find(options.begin(), options.end(), option);

    while (it == options.end()) {
        cout << "########################################################################################################\n";
        cout << "#                                                                                                      #\n";
        cout << "#                                       WELCOME TO MOVIE STREAMING                                     #\n";
        cout << "#                                           ~ Your Movie World ~                                       #\n";
        cout << "#                                                                                                      #\n";
        cout << "########################################################################################################\n";
        cout << "#                                                                                                      #\n";
        cout << "#      ________                   ________                 ________             ________               #\n";
        cout << "#     |        |                 |        |               |        |            |       |              #\n";
        cout << "#     |  MOVIE |                 | MOVIE  |               | MOVIE  |            | MOVIE |              #\n";
        cout << "#     | POSTER |                 | POSTER |               | POSTER |            | POSTER|              #\n";
        cout << "#     |_______ |                 |________|               |________|            |_______|              #\n";
        cout << "#                                                                                                      #\n";
        cout << "#                  Trending Now:         Watch Again:         Recommended for You:                     #\n";
        cout << "#                   ______________          ______________          ______________                     #\n";
        cout << "#                  |              |        |              |        |              |                    #\n";
        cout << "#                  |   MOVIE A    |        |   MOVIE B    |        |   MOVIE C    |                    #\n";
        cout << "#                  |______________|        |______________|        |______________|                    #\n";
        cout << "#                                                                                                      #\n";
        cout << "# ---------------------------------------------------------------------------------------------------- #\n";
        cout << "#                                                                                                      #\n";
        cout << "#                         [1] Search Movies  |  [2] Browse by Tags  |  [3] Watch Later                 #\n";
        cout << "#                                                                                                      #\n";
        cout << "#                                  [4] View Liked Movies   |   [5] Exit                                #\n";
        cout << "#                                                                                                      #\n";
        cout << "########################################################################################################\n";
        cout << "#                                 Select an option to continue...                                      #\n";
        cout << "########################################################################################################\n\n";
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
                AsignedMovies(mapa_ids , trieTitle, trieSynopsis, trieTags, duration, results);
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
            AsignedMovies(mapa_ids , trieTitle, trieSynopsis, trieTags, duration, results);
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
