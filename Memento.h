//
// Created by david on 30/11/2024.
//
#include <vector>
#include <iostream>
#include <Movie.h>
using namespace std;
#ifndef MEMENTO_H
#define MEMENTO_H

class Memento {
private:
    vector<Movie*> current5movies;
public:
    explicit Memento(vector<Movie*> &movies5) {
        current5movies = movies5;
    }

    void order() {
        // Ordenar el vector de mayor a menor peso
        sort(current5movies.begin(), current5movies.end(), []( Movie* a,  Movie* b) {
            return a->verifyPeso() > b->verifyPeso(); // Comparar de mayor a menor
        });
    }

    string getState() {
        // Generar el string de salida con las películas ordenadas
        order();
        string result = "Peliculas ordenadas:\n";
        for (size_t i = 0; i < current5movies.size(); ++i) {
            result += to_string(i + 1) + ". " + current5movies[i]->getTitulo() +
                      " (Peso: " + to_string(current5movies[i]->verifyPeso()) + ")\n";
        }

        return result;

    }

    void like(int num) {
        current5movies[num]->givelike();
        cout<<"Le has dado like a la pelicula -> "<<current5movies[num]->getTitulo()<<endl;
    }

    void later(int num) {
        current5movies[num]->addLater();
        cout<<"Has agregado a ver mas tarde a la pelicula -> "<<current5movies[num]->getTitulo()<<endl;
    }


    string getSynopsis(int num) const{
        string str2;
        string like = current5movies[num]->haslike() ? "SI" : "NO";
        string later = current5movies[num]->hasLater() ? "SI" : "NO";
        str2 = current5movies[num]->getsinopsis() + "\n" + "Tiene like: " + like + "\n" + "Ver mas tarde: " + later;
        return str2;
    }

    string getMovieId(int num) const{
        return current5movies[num] -> getId();
    }

    string getMovieTitle(int num) const{
        return current5movies[num] -> getTitulo();
    }

    ~Memento();
};

Memento::~Memento() = default;

#endif //MEMENTO_H
