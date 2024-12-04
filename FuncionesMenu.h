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
    cout << "Ingrese el numero: ";
}
// ----

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
void callMenuAgain(TrieNode& trieTitle, TrieNode& trieSynopsis, TrieNode& trieTags,
               const unordered_map<string, Movie*>& mapa_ids,
               chrono::duration<double> duration) {
    char e; cin>>e;
    showMenu(trieTitle,trieSynopsis,trieTags,mapa_ids,duration);
}

#endif //FUNCIONES_MENU_H
