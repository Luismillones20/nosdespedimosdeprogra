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

void callMenuAgain(TrieNode& trieTitle, TrieNode& trieSynopsis, TrieNode& trieTags,
               const unordered_map<string, Movie*>& mapa_ids,
               chrono::duration<double> duration) {
    char e; cin>>e;
    showMenu(trieTitle,trieSynopsis,trieTags,mapa_ids,duration);
}

#endif //FUNCIONES_MENU_H
