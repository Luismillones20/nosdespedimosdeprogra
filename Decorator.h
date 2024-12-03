//
// Created by david on 03/12/2024.
//

#ifndef DECORATOR_H
#define DECORATOR_H
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Movie.h"

using namespace std;

// Interfaz base para los estilos de visualización
class Decorator {
public:
    virtual void display(const vector<string>& movieIds, const unordered_map<string, Movie*>& movieMap) const = 0;
    virtual ~Decorator() = default;
};

// Decorador concreto para mostrar los likes
class LikeDecorator : public Decorator {
public:
    void display(const vector<string>& movieIds, const unordered_map<string, Movie*>& movieMap) const override {
        cout << "\n======================================================\n";
        cout << "                🌟 Peliculas con Like 🌟   \n";
        cout << "======================================================\n";
        for (const auto& id : movieIds) {
            if (movieMap.find(id) != movieMap.end()) {
                cout << "🎥 " << movieMap.at(id)->getTitulo() << " | ID: " << id << endl;
            }
        }
        cout << "===========================\n";
    }
};

// Decorador concreto para mostrar las películas para ver más tarde
class VerMasTardeDecorator : public Decorator {
public:
    void display(const vector<string>& movieIds, const unordered_map<string, Movie*>& movieMap) const override {
        cout << "\n======================================================\n";
        cout << "                  ⏳ Ver Mas Tarde ⏳   \n";
        cout << "======================================================\n";
        for (const auto& id : movieIds) {
            if (movieMap.find(id) != movieMap.end()) {
                cout << "📽️ " << movieMap.at(id)->getTitulo() << " | ID: " << id << endl;
            }
        }
        cout << "===========================\n";
    }
};
#endif //DECORATOR_H
