#include "menu.h"
#include <iostream>
#include <chrono>
#include <set>
#include <algorithm>
#include <sstream>
#include <Historial.h>
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
    string line, id, c, nombre;
    getline(file, line);
    cout<<"*** Lista de likes *** "<<endl;
    while (true) {
        file>>id;
        if(file.eof()) break;
        file>>ws>>c>>ws;
        getline(file, nombre);
        cout<<id<<" "<<c<<" "<<nombre<<endl;
    }
    cout<<endl;
    file.close();
}

void showWatchLater(ifstream& file){
    string line, id, c, nombre;
    getline(file, line);
    cout<<"*** Lista para Ver Mas Tarde *** "<<endl;
    while (true) {
        file>>id;
        if(file.eof()) break;
        file>>ws>>c>>ws;
        getline(file, nombre);
        cout<<id<<" "<<c<<" "<<nombre<<endl;
    }
    cout<<endl;
    file.close();
}

void saveLikes(string likedId, string likedTitle,vector<string>& vecLikedIds,vector<string>& vecLikedTitles){
    //Primero, leer archivo con peliculas likeadas:

    ifstream archLikesLeer("../listaLikes.txt", ios::in);
    if (!archLikesLeer.is_open()) {
        // Si el archivo de ganadores No existe:
        cout << "No se pudo encontrar un registro de ganadores existente" << endl;
        cout << "Creando nueva lista de ganadores..." << endl;

        // Dar valor inicial al vector agregando la nueva pelicula likeada:
        vecLikedIds.emplace_back(likedId);
        vecLikedTitles.emplace_back(likedTitle);

    } else { // Si el archivo de ganadores Sí existe:
        //Detectar si existe la película likeada en el archivo. Si está, dejarla así. Si no, la añade.

        //Primero, llenar los vectores con las peliculas likeadas existentes:
        string linea, id, c, title;
        bool foundMovieId = false;
        getline(archLikesLeer, linea); // Leyendo la primera linea de "Lista de likes:"
        while (true) {
            archLikesLeer>>id;
            if(archLikesLeer.eof()) break;
            archLikesLeer>>ws>>c>>ws;
            getline(archLikesLeer, title);
            vecLikedIds.emplace_back(id);
            vecLikedTitles.emplace_back(title);
        }

        // Ahora, recorrer el vector de nombre buscando al id de la pelicula:
        for (int i = 0; i < vecLikedIds.size(); i++) {
            if (likedId == vecLikedIds[i]) {
                foundMovieId = true; // Si lo encuentra,
                break; // deja de buscar
            }
        }
        // Si no lo encuentra, lo añade
        if (!foundMovieId) {
            vecLikedIds.emplace_back(likedId);
            vecLikedTitles.emplace_back(likedTitle);
        }

        // Ya están los vectores actualizados con las películas likeadas (ids y títulos).
        // Ahora, a escribirlos en el archivo listaLikes.txt

    }
    archLikesLeer.close(); // Cerramos el archivo (es buena práctica)

    // Ahora, a escribir el archivo (siempre se sobreescribe)
    ofstream archLikesEscribir("../listaLikes.txt", ios::out);

    archLikesEscribir<<"Lista de likes: "<<endl;
    for (int i = 0; i < vecLikedIds.size(); i++) {
        archLikesEscribir << vecLikedIds[i]<<" - "<<vecLikedTitles[i]<<endl;
    }
    archLikesEscribir.close(); // Cerramos el archivo (es buena práctica)
}

void saveWatchLater(string watchL8rId, string watchL8rTitle,vector<string>& vecIds,vector<string>& vecTitles){
    //Primero, leer archivo con peliculas likeadas:

    ifstream archVerMasTardeLeer("../listaVerMasTarde.txt", ios::in);
    if (!archVerMasTardeLeer.is_open()) {
        // Si el archivo de ganadores No existe:
        cout << "No se pudo encontrar un registro de peliculas para ver mas tarde existente" << endl;
        cout << "Creando nueva lista..." << endl;

        // Dar valor inicial al vector agregando la nueva pelicula likeada:
        vecIds.emplace_back(watchL8rId);
        vecTitles.emplace_back(watchL8rTitle);

    } else { // Si el archivo Sí existe:
        //Detectar si existe la película marcada en el archivo. Si está, dejarla así. Si no, la añade.

        //Primero, llenar los vectores con las peliculas con Ver mas tarde existentes:
        string linea, id, c, title;
        bool foundMovieId = false;
        getline(archVerMasTardeLeer, linea); // Leyendo la primera linea de "Lista para Ver mas tarde:"
        while (true) {
            archVerMasTardeLeer>>id;
            if(archVerMasTardeLeer.eof()) break;
            archVerMasTardeLeer>>ws>>c>>ws;
            getline(archVerMasTardeLeer, title);
            vecIds.emplace_back(id);
            vecTitles.emplace_back(title);
        }

        // Ahora, recorrer el vector de nombre buscando al id de la pelicula:
        for (int i = 0; i < vecIds.size(); i++) {
            if (watchL8rId == vecIds[i]) {
                foundMovieId = true; // Si lo encuentra,
                break; // deja de buscar
            }
        }
        // Si no lo encuentra, lo añade
        if (!foundMovieId) {
            vecIds.emplace_back(watchL8rId);
            vecTitles.emplace_back(watchL8rTitle);
        }

        // Ya están los vectores actualizados con las películas para Ver mas tarde (ids y títulos).
        // Ahora, a escribirlos en el archivo listaVerMasTarde.txt

    }
    archVerMasTardeLeer.close(); // Cerramos el archivo (es buena práctica)

    // Ahora, a escribir el archivo (siempre se sobreescribe)
    ofstream archVerMasTardeEscribir("../listaVerMasTarde.txt", ios::out);

    archVerMasTardeEscribir<<"Lista para Ver mas tarde: "<<endl;
    for (int i = 0; i < vecIds.size(); i++) {
        archVerMasTardeEscribir << vecIds[i]<<" - "<<vecTitles[i]<<endl;
    }
    archVerMasTardeEscribir.close(); // Cerramos el archivo (es buena práctica)
}

void llenarVecLikesExistentes(vector<string>& vecLikesExistentes){
    ifstream archLikesExistentesLeer("../listaLikes.txt", ios::in);
    if (!archLikesExistentesLeer.is_open()) {
        // Si el archivo de likes No existe:
        cout << "No se pudo encontrar un registro de likes existente" << endl;
    }else{
        string line, id, c, nombre;
        getline(archLikesExistentesLeer, line);
        while (true) { // Si el archivo Sí existe
            archLikesExistentesLeer>>id;
            if(archLikesExistentesLeer.eof()) break;
            archLikesExistentesLeer>>ws>>c>>ws;
            getline(archLikesExistentesLeer, nombre);
            vecLikesExistentes.emplace_back(id);
        }
        archLikesExistentesLeer.close();
    }
}

void llenarVecForLaterExistentes(vector<string>& vecForLaterExistentes){
    ifstream archforLaterExistentesLeer("../listaVerMasTarde.txt", ios::in);
    if (!archforLaterExistentesLeer.is_open()) {
        // Si el archivo de Ver mas tarde No existe:
        cout << "No se pudo encontrar un registro de Para ver mas tarde existente" << endl;
    }else{ // Si el archivo Sí existe
        string line, id, c, nombre;
        getline(archforLaterExistentesLeer, line);
        while (true) {
            archforLaterExistentesLeer>>id;
            if(archforLaterExistentesLeer.eof()) break;
            archforLaterExistentesLeer>>ws>>c>>ws;
            getline(archforLaterExistentesLeer, nombre);
            vecForLaterExistentes.emplace_back(id);
        }
        archforLaterExistentesLeer.close();
    }
}

void modifyLikesAndForLater(Movie* &movie, const string& id,
                            vector<string>& vecLikesExistentes, vector<string>& vecForLaterExistentes){
    for(auto v : vecLikesExistentes){
        if(v == id){
            movie->setLike(true);
            int p = movie->getPeso();
            movie->setPeso(p+5);
            break;
        }
    }
    for(auto v : vecForLaterExistentes){
        if(v == id){
            movie->setForLater(true);
            int p = movie->getPeso();
            movie->setPeso(p+3);
            break;
        }
    }
}

template <typename... Vectors>
void AsignedMovies(const unordered_map<string, Movie*>& mapa,
                   TrieNode& trieTitle,
                   TrieNode& trieSynopsis,
                   TrieNode& trieTags,
                   chrono::duration<double> duration,
                   const Vectors&... vector_ids) {
    set<string> printed_titles;       // Evitar duplicados
    bool exit_loop = false;          // Controlar la salida del bucle principal
    vector<Movie*> movies_5; // Almacena las películas actuales
    Historial historial;             // Objeto para manejar los estados
    int counter = 0;                 // Contador para gestionar estados del historial

    vector<string> vecLikesExistentes;
    vector<string> vecForLaterExistentes;

    llenarVecLikesExistentes(vecLikesExistentes);
    llenarVecForLaterExistentes(vecForLaterExistentes);

    // Lambda para procesar y cargar las películas
    auto process_id = [&](const string& id) -> bool {
        auto it = mapa.find(id);
        if (it != mapa.end() && printed_titles.insert(it->second->getTitulo()).second) {
            Movie* movie = it->second;
            //Agregar valores de Likeado y Ver mas tarde:
            modifyLikesAndForLater(movie, id, vecLikesExistentes, vecForLaterExistentes);

            movies_5.push_back(movie);
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

    //Un solo algoritmo para meter peliculas y ordenarlas en tiempo real
    auto actualizarHistorial = [&]() {
        // Ordenar todas las películas globalmente por peso
        sort(movies_5.begin(), movies_5.end(), [](Movie* a, Movie* b) {
            return a->verifyPeso() > b->verifyPeso();
        });
        // Limpiar el historial y regenerar mementos
        historial.clearEstados();
        vector<Movie*> group;
        for (size_t i = 0; i < movies_5.size(); ++i) {
            group.push_back(movies_5[i]);
            if (group.size() == 5 || i == movies_5.size() - 1) {
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
                    vector<string> vecIds; // Vector con ids de películas con like
                    vector<string> vecTitles; // Vector con ids de películas con like
                    if(e==1){
                        estado_actual.like(option-1);
                        // Guardando el id de la película likeada
                        string likedId, likedTitle;
                        likedId = estado_actual.getMovieId(option-1);
                        likedTitle = estado_actual.getMovieTitle(option-1);
                        saveLikes(likedId, likedTitle, vecIds, vecTitles);
                    }
                    else if(e==2){
                        estado_actual.later(option-1);
                        // Guardando el id de la película para Ver Despues
                        string watchL8rId, watchL8rTitle;
                        watchL8rId = estado_actual.getMovieId(option-1);
                        watchL8rTitle = estado_actual.getMovieTitle(option-1);
                        saveWatchLater(watchL8rId, watchL8rTitle, vecIds, vecTitles);
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
        } else if(option == 3){
            ifstream archVerMasTardeLeer("../listaVerMasTarde.txt",ios::in);
            if (!archVerMasTardeLeer.is_open()) {
                cout << "No se han registrado likes." << endl;
            }else{
                showWatchLater(archVerMasTardeLeer);
            }
        }else if (option == 4){
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
