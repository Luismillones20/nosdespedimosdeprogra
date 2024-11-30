//
// Created by david on 30/11/2024.
//

#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;
class Archivo {
private:
    static unique_ptr<Archivo> instancia;
    ifstream file;
    string directory;

    Archivo(string namefile):directory(namefile) {
        file.open(namefile, ios::in);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo" << endl;
        }
    }


public:
    Archivo(const Archivo&) = delete;
    Archivo& operator=(const Archivo&) = delete;

    static Archivo& getArchivo(const string& filename = "../mpst_full_data (1).csv") {
        if(!instancia){
            if(filename.empty()){
                throw runtime_error("El archivo no se encontro");
            }
            instancia = unique_ptr<Archivo>(new Archivo(filename)) ;
        }
        return *instancia;
    }

    ifstream& getFile() {
        return file;
    }

    void close() {
        if(file.is_open()){
            file.close();
        }
        cout<<"Archivo cerrado"<<endl;
    }

    ~Archivo(){close();}

};

unique_ptr<Archivo> Archivo::instancia = nullptr;


#endif //ARCHIVO_H
