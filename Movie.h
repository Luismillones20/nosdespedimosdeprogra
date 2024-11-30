//
// Created by david on 30/11/2024.
//

#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <utility>
using namespace std;
class Movie {
private:
    string id;
    string name;
    string sinopsis;
public:
    Movie(){}
    Movie(string id_, string name_, string sino_): id(std::move(id_)), name(std::move(name_)),sinopsis(std::move(sino_)){
    }

    string getid() const {
        return id;
    }

    string getname() const {
        return name;
    }

    string getsinopsis() const {
        return sinopsis;
    }

    ~Movie() = default;

};
#endif //MOVIE_H
