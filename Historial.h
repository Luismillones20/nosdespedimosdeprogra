//
// Created by david on 30/11/2024.
//
#include <vector>
#include <iostream>
#include <Memento.h>
using namespace std;
#ifndef HISTORIAL_HHi
#define HISTORIAL_H

class Historial {
private:
    vector<Memento> estados;

public:
    void agregarEstado(const Memento& estado) {
        estados.push_back(estado);
    }

    Memento getEstadoAnterior() {
        if (estados.size() > 1) {
            estados.pop_back();
            return estados.back();
        }
        //return Memento();

    }
    /*
    Memento getEstadoActual(string count){
        return estados[estados.size()-1];
    }*/
    void printEstadoActual(int &counter) {
        if (counter >= 0 && counter < static_cast<int>(estados.size())) {
            cout << estados[counter].getState();
        } else {
            cout << "No hay más películas para mostrar.\n";
        }
    }

    const vector<Memento>& getter_estados() const {
        return estados;
    }
    //~Historial();
};

#endif //HISTORIAL_H
