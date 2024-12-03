//
// Created by david on 30/11/2024.
//

#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <utility>
using namespace std;
//Que se implementara para el archivo Observer
class Observer {
public:
    virtual void update(const string& movieId, const string& action) = 0;
    virtual ~Observer() = default;
};

class Movie {
private:
    string id;
    string name;
    string sinopsis;
    int peso = 0;
    bool like = false;
    bool forLater = false;
    vector<Observer*> observers; // Lista de observadores
public:
    Movie(){}
    Movie(string id_, string name_, string sino_): id(std::move(id_)), name(std::move(name_)),sinopsis(std::move(sino_)){
    }

    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void notifyObservers(const string& action) {
        for (auto* observer : observers) {
            observer->update(id, action);
        }
    }

    // Métodos originales con notificación
    void givelike() {
        if (!like) {
            like = true;
            peso += 5;
            notifyObservers("like");
        }
    }

    void addLater() {
        if (!forLater) {
            forLater = true;
            peso += 3;
            notifyObservers("addLater");
        }
    }

    int verifyPeso() {
        return peso;
    }

    bool hasLater() {
        return forLater;
    }
    bool haslike() {
        return like;
    }

    string getId() const {
        return id;
    }

    string getTitulo() const {
        return name;
    }

    string getsinopsis() const {
        return sinopsis;
    }

    int getPeso() const {
        return peso;
    }

    void setPeso(int peso) {
        this->peso = peso;
    }

    void setLike(bool like) {
        this->like = like;
    }

    void setForLater(bool forLater) {
        this->forLater = forLater;
    }

    ~Movie() = default;

};
#endif //MOVIE_H
