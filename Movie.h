//
// Created by david on 30/11/2024.
//

#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <set>
#include <utility>
using namespace std;

class Movie {
private:
    string id;
    string name;
    string sinopsis;
    int peso = 0;
    bool like = false;
    bool forLater = false;
public:
    Movie(){}
    Movie(string id_, string name_, string sino_): id(std::move(id_)), name(std::move(name_)),sinopsis(std::move(sino_)){
    }

    int verifyPeso() {
        return peso;
    }

    void givelike() {
        like = true;
        peso+=5;
    }

    void addLater() {
        forLater = true;
        peso+=3;
    }

    bool hasLater() {
        return forLater;
    }
    bool haslike() {
        return like;
    }

    string getid() const {
        return id;
    }

    string getTitulo() const {
        return name;
    }

    string getsinopsis() const {
        return sinopsis;
    }

    ~Movie() = default;

};
#endif //MOVIE_H
