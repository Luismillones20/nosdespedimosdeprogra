#include "menu.h"
#include <iostream>
#include <chrono>
#include <set>
#include <algorithm>
#include <sstream>
#include <Historial.h>
#include <fstream>
#include "Movie.h"
#include <FuncionesMenu.h>
#include "Observer.h"
using namespace std;

template <typename... Vectors>
void AsignedMovies(const unordered_map<string, Movie*>& mapa, // Mapa <id, Movie>
                   TrieNode& trieTitle,                       // Arbol de Titles
                   TrieNode& trieSynopsis,                    // Arbol de Synopsis
                   TrieNode& trieTags,                        // Arbol de Tags
                   chrono::duration<double> duration,
                   FavoritesManager& favManager,
                   const Vectors&... vector_ids) {            // Cuantos vectores lleguen
    set<string> printed_titles;       // Titulos mostrados. Se ussa "set" para evitar duplicados
    bool exit_loop = false;           // Controlar la salida del bucle principal
    vector<Movie*> movies_5;          // Almacena las películas actuales
    Historial historial;              // Objeto para manejar los estados
    int counter = 0;                  // Contador para gestionar estados del historial

    vector<string> vecLikesExistentes;
    vector<string> vecForLaterExistentes;

    llenarVecLikesExistentes(vecLikesExistentes);
    llenarVecForLaterExistentes(vecForLaterExistentes);

    // Lambda para procesar y cargar las películas
    auto process_id = [&](const string& id) -> bool {
        auto it = mapa.find(id);
        // Devuelve un iterator con la posicion de ese id en el mapa <id, Movie>

        if (it != mapa.end() && printed_titles.insert(it->second->getTitulo()).second) {
            // Si se encontró el id, y y se pudo insertar el titulo de esa pelicula en el set "printed_titles",

            // Se crea una instancia de Movie y se le asigna los valores de la pelicula encontrada en el mapa:
            Movie* movie = it->second;

            //Agregar valores de Likeado y Ver mas tarde:
            modifyLikesAndForLater(movie, id, vecLikesExistentes, vecForLaterExistentes);

            if (favManager.isLiked(movie->getId())) {
                movie->setLike(true);
                movie->setPeso(movie->getPeso() + 5);
            }
            if (favManager.isWatchLater(movie->getId())) {
                movie->setForLater(true);
                movie->setPeso(movie->getPeso() + 3);
            }

            // Se agrega la pelicula al vector de movies_5 (para juntar las 5 peliculas de la página a mostrar)
            movies_5.push_back(movie);
        }
        return true;
    };

    // Procesar todos los vectores. Función sin nombre que se ejecuta automáticamente
    ([&]() {
        for (const auto& id : vector_ids) {
            if (exit_loop) break; // Romper el bucle si se activa `exit_loop`
            process_id(id);      // Procesar cada ID individual
        }
    }(),...); // Expansión del pack para aplicar a cada vector

    //Un solo algoritmo para meter peliculas y ordenarlas en tiempo real
    auto actualizarHistorial = [&]() {
        // Ordenar todas las películas globalmente por peso
        sort(movies_5.begin(), movies_5.end(), [](Movie* a, Movie* b) { // Se ordenan las Movies en el vector
            return a->verifyPeso() > b->verifyPeso();                   // mediante el peso que llevan
        });
        // Limpiar el historial y regenerar mementos
        historial.clearEstados();
        vector<Movie*> group;
        for (size_t i = 0; i < movies_5.size(); ++i) {
            group.push_back(movies_5[i]);
            if (group.size() == 5 || i == movies_5.size() - 1) {        // Cada 5 peliculas || cuando el vector se acaba
                Memento memento(group);
                historial.agregarEstado(memento);
                group.clear();  // Limpiar el grupo para la próxima iteración
            }
        }
    };

    // Ordenar por primera vez y generar historial
    actualizarHistorial();

    // Bucle principal para interactuar con el usuario
    int option = 0;
    while (!exit_loop) {
        GenerateSpaces();
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "                                     Peliculas disponibles:\n";
        cout << "------------------------------------------------------------------------------------------------------\n";

        // Mostrar el estado actual del historial
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
        cout << "Ingrese el numero: ";
        cin >> option;
        if (option >= 1 && option <= 5) {
            // Mostrar sinopsis de la película seleccionada
            auto& estados = historial.getter_estados();
            if (!estados.empty() && counter < static_cast<int>(estados.size())) {
                Memento& estado_actual = estados[counter];
                showSynopsis( estado_actual.getSynopsis(option - 1));
                int e;
                cin >> e;
                while(e == 1 || e==2){
                    string movieId = estado_actual.getMovieId(option - 1);
                    // vector<string> vecIds; // Vector con ids de películas con like
                    // vector<string> vecTitles; // Vector con ids de películas con like
                    if(e==1){
                        estado_actual.like(option-1);
                        // Guardando el id de la película likeada
                        // string likedId, likedTitle;
                        // likedId = estado_actual.getMovieId(option-1);
                        // likedTitle = estado_actual.getMovieTitle(option-1);
                        // saveLikes(likedId, likedTitle, vecIds, vecTitles);
                        favManager.likeMovie(movieId);
                    }
                    else if(e==2){
                        estado_actual.later(option-1);
                        // Guardando el id de la película para Ver Despues
                        // string watchL8rId, watchL8rTitle;
                        // watchL8rId = estado_actual.getMovieId(option-1);
                        // watchL8rTitle = estado_actual.getMovieTitle(option-1);
                        // saveWatchLater(watchL8rId, watchL8rTitle, vecIds, vecTitles);
                        favManager.addToWatchLater(movieId);
                    }
                    showSynopsis(estado_actual.getSynopsis(option - 1));
                    cin >> e;
                }
                // **Actualizar pesos y reordenar dinámicamente**
                actualizarHistorial();

                // Ajustar el índice del historial después de reordenar
                counter = min(counter, static_cast<int>(historial.getter_estados().size()) - 1);
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
               chrono::duration<double> duration
) {
    auto* favManager = new FavoritesManager(mapa_ids);
    auto* like_decorator = new LikeDecorator();
    auto* vmt_decorator = new VerMasTardeDecorator();
    int option = 0;
    vector<int> options = {1, 2, 3, 4, 5, 6};
    auto it = find(options.begin(), options.end(), option);
    // Iterador para luego verificar si la opcion ingresada está dentro de las posibilidades

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
                //results es un vector de ids de las peliculas donde en sus titulos salen las palabras que has tipeado
                vector<string> results = trieTitle.searchByPrefix(word);
                vector<string> results2 = trieSynopsis.searchByPrefix(word);

                //Para asignar las películas que tienen esos ids. También muestra esas películas
                AsignedMovies(mapa_ids, trieTitle, trieSynopsis, trieTags, duration, *favManager, results);
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
            AsignedMovies(mapa_ids, trieTitle, trieSynopsis, trieTags, duration, *favManager, results);
        } else if(option == 3){
            // ifstream archVerMasTardeLeer("../listaVerMasTarde.txt",ios::in);
            // if (!archVerMasTardeLeer.is_open()) {
            //     cout << "No se han registrado Ver Mas Tarde." << endl;
            // }else{
            //     showWatchLater(archVerMasTardeLeer);
            //     archVerMasTardeLeer.close();
            // }
            favManager->showMasTardeDecorator(vmt_decorator);
            callMenuAgain(trieTitle,trieSynopsis, trieTags,mapa_ids,duration);
        }else if (option == 4){
            // ifstream archLikesLeer("../listaLikes.txt", ios::in);
            // if (!archLikesLeer.is_open()) {
            //     cout << "No se han registrado likes." << endl;
            // } else {
            //     showLikes(archLikesLeer);
            //     archLikesLeer.close();
            // }
            favManager->showLikesDecorator(like_decorator);
            callMenuAgain(trieTitle,trieSynopsis, trieTags,mapa_ids,duration);
        } else if (option == 5) {
            GenerateSpaces();
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                Gracias por usar Movie Streaming. Hasta luego!" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            break;
        } else if(option == 6){
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                   Presiona 6 para volver al Menu Principal                     " << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            cin>>option;
        }
        else {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "                           Opción no válida. Intente nuevamente.\n";
            cout << "--------------------------------------------------------------------------------" << endl;
        }
        it = find(options.begin(), options.end(), option);
    }
}
