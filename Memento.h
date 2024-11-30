//
// Created by david on 30/11/2024.
//
#include <vector>
#include <iostream>
using namespace std;
#ifndef MEMENTO_H
#define MEMENTO_H

class Memento {
private:
    vector<pair<string, string>> current5movies;
public:
    //Memento();
    explicit Memento(vector<pair<string, string>> &movies5) {
        current5movies = movies5;
    } // despues sino le ponemos const

    string getState() const {
        string str;
        for (int i = 0; i < 5; i ++){
            str += to_string(i+1) + ". " + current5movies[i].first + "\n";
        }
        return str;
    }
    string getSynopsis(int num) const{
        string str2;
        str2 = current5movies[num].second + "\n";
        return str2;
    }
    ~Memento();
};

Memento::~Memento() = default;

#endif //MEMENTO_H
