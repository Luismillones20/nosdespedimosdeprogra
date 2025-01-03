//
// Created by david on 30/11/2024.
//

#ifndef HISTORIAL_HHi
#define HISTORIAL_H
#include <vector>
#include <iostream>
#include <Memento.h>

#include <mutex>
std::mutex historial_mutex;


using namespace std;

class Historial {
private:
    vector<Memento> estados;

public:
    void agregarEstado(const Memento& estado) {
        estados.push_back(estado);
    }

    void orderEstados() {
        for(auto estado: estados){
            estado.order();
        }
    }

    void clearEstados() {
        estados.clear();
    }

    Memento getEstadoAnterior() {
        if (estados.size() > 1) {
            estados.pop_back();
            return estados.back();
        }
    }
    /*
    Memento getEstadoActual(string count){
        return estados[estados.size()-1];
    }*/
    void printEstadoActual(int &counter) {
        if (counter >= 0 && counter < estados.size()) {
            cout << estados[counter].getState();
        } else {
            cout << "No hay más películas para mostrar.\n";
        }
    }

     vector<Memento>& getter_estados() {
        return estados;
    }
    //~Historial();
};


void threadSafeAddToHistorial(Historial& historial, const Memento& memento) {
    std::lock_guard<std::mutex> lock(historial_mutex);
    historial.agregarEstado(memento);
}
#endif //HISTORIAL_H
