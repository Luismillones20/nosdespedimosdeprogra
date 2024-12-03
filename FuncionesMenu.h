//
// Created by david on 01/12/2024.
//

#ifndef FUNCIONES_MENU_H
#define FUNCIONES_MENU_H

#include "menu.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include "Movie.h"
#include <FuncionesMenu.h>
using namespace std;

void GenerateSpaces(){
    for (int i = 0; i < 40; i++) cout << "\n";
}

void showSynopsis(string sinopsis) {
    GenerateSpaces();
    cout << "Sinopsis:\n" << sinopsis << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                 1.Dar Like            2.Agregar a Ver Mas Tarde                " << endl;
    cout << "        Presione una numero diferente para poder volver a la pagina anterior    " << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}
// ----
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
void callMenuAgain(TrieNode& trieTitle, TrieNode& trieSynopsis, TrieNode& trieTags,
               const unordered_map<string, Movie*>& mapa_ids,
               chrono::duration<double> duration) {
    char e; cin>>e;
    showMenu(trieTitle,trieSynopsis,trieTags,mapa_ids,duration);
}

#endif //FUNCIONES_MENU_H
