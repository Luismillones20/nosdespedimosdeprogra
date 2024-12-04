#ifndef FAVORITES_MANAGER_H
#define FAVORITES_MANAGER_H

#include <unordered_map>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

#include "Decorator.h"
#include "Movie.h"

using namespace std;

class FavoritesManager : public Observer{
    unordered_map<string, Movie*> movieMap; // Mapa de ID a Movie
    set<string> favorites;                 // IDs de películas con like
    set<string> watchLater;                // IDs de películas para ver más tarde
    const string likesFile = "../listaLikes.txt";
    const string watchLaterFile = "../listaVerMasTarde.txt";

public:
    void update(const string& movieId, const string& action) override{};
    FavoritesManager(unordered_map<string, Movie*> movieMap_) : movieMap(std::move(movieMap_)) {
        loadLikes();
        loadWatchLater();
    }

    // Verificar si una película está likeada
    bool isLiked(const string& movieId) const {
        return favorites.find(movieId) != favorites.end();
    }

    // Verificar si una película está en la lista de "ver más tarde"
    bool isWatchLater(const string& movieId) const {
        return watchLater.find(movieId) != watchLater.end();
    }

    // Agregar película a la lista de likes
    void likeMovie(const string& movieId) {
        if (movieMap.find(movieId) != movieMap.end()) {
            favorites.insert(movieId);
            saveLikes();
        }
    }

    // Agregar película a la lista de "ver más tarde"
    void addToWatchLater(const string& movieId) {
        if (movieMap.find(movieId) != movieMap.end()) {
            watchLater.insert(movieId);
            saveWatchLater();
        }
    }

    // Mostrar películas con like
    void showFavorites() const {
        cout << "Lista de películas con like:\n";
        for (const auto& id : favorites) {
            cout << "ID: " << id << " | Titulo: " << movieMap.at(id)->getTitulo() << endl;
        }
    }

    // Mostrar películas "ver más tarde"
    void showForLater() const {
        cout << "Lista de peliculas para ver mas tarde:\n";
        for (const auto& id : watchLater) {
            cout << "ID: " << id << " | Titulo: " << movieMap.at(id)->getTitulo() << endl;
        }
    }

    void loadLikes() {
        ifstream inFile(likesFile, ios::in);
        if (!inFile.is_open()) {
            cout << "         No se encontro el archivo de Likes " << endl;
            cout << "Se creara uno nuevo al agregar una pelicula a la lista."<<endl;
            return;
        }

        string line, id, title;
        getline(inFile, line); // Saltar encabezado
        while (getline(inFile, line)) {
            size_t sep = line.find(" - ");
            id = line.substr(0, sep);
            if (movieMap.find(id) != movieMap.end()) {
                favorites.insert(id);
                movieMap[id]->setLike(true);
                movieMap[id]->setPeso(movieMap[id]->getPeso() + 5);
            }
        }
        inFile.close();
    }

    void loadWatchLater() {
        ifstream inFile(watchLaterFile, ios::in);
        if (!inFile.is_open()) {
            cout << "      No se encontro el archivo de 'Ver mas Tarde' " << endl;
            cout << "Se creara uno nuevo al agregar una pelicula a la lista."<<endl;
            return;
        }

        string line, id, title;
        getline(inFile, line); // Saltar encabezado
        while (getline(inFile, line)) {
            size_t sep = line.find(" - ");
            id = line.substr(0, sep);
            if (movieMap.find(id) != movieMap.end()) {
                watchLater.insert(id);
                movieMap[id]->setForLater(true);
                movieMap[id]->setPeso(movieMap[id]->getPeso() + 3);
            }
        }
        inFile.close();
    }

    void saveLikes() const {
        ofstream outFile(likesFile, ios::out);
        outFile << "Lista de likes:\n";
        for (const auto& id : favorites) {
            outFile << id << " - " << movieMap.at(id)->getTitulo() << endl;
        }
        outFile.close();
    }

    void saveWatchLater() const {
        ofstream outFile(watchLaterFile, ios::out);
        outFile << "Lista para ver mas tarde:\n";
        for (const auto& id : watchLater) {
            outFile << id << " - " << movieMap.at(id)->getTitulo() << endl;
        }
        outFile.close();
    }

    //Para implementar el decorator:
    void showLikesDecorator(const Decorator* style) const {
        vector<string> favoriteIds(favorites.begin(), favorites.end());
        style->display(favoriteIds, movieMap);
    }

    void showMasTardeDecorator(const Decorator* style) const {
        vector<string> watchLaterIds(watchLater.begin(), watchLater.end());
        style->display(watchLaterIds, movieMap);
    }

};

#endif // FAVORITES_MANAGER_H
