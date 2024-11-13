#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include "trie.h"

// constexpr int N = 256;
using namespace std;
// TAREAS:
// 2. Que tenga una funcion thread que recorra una parte y otro otra
// 3. Ponerle un sistema de puntuación en el cual se elija que pelicula debe ir primero
// 4. Sistema de likes (recomendar peliculas similiares), igual con ver mas tarde
// 5. Usar el singleton, iterator, memento
// 6. Busqueda por varias palabras
// 7. Imprimir 5 peliculas en orden, y luego un boton para las siguientes 5 peliculas
// 8. Busqueda por tag
// 9. Arreglar el tema de que no recorre ciertas palabras




// SOLUCIONES EN MENTE:


// 2
// 7. El like tendria un peso de 5, el ver mas tarde un peso de 3, se compararía cuales tienen mayor cantidad y se pondrían, basicamente un if si es que tiene un ver mas tarde o un like se pondría al inicio
// 5. En el caso de singleton se pondría a la base de datos
//      En el caso de memento se pondría para recordar las busquedas que ha realizado
// 6. El más sencillo hasta ahora, poner q en lugar de un string reciba varios.
// 10.
// 8. En la busqueda por tag se tendria que comparar si es que está el tag se podria poner tuplas en lugar de pairs, en este caso if tupla pertenece a peliculas se pone la pelicula,
// 9.


// EXTRA:
// Que se imprima en pantalla

void GetCorrectGetLines(ifstream  &file, string &temp) {
    bool insideQuotes = false;
    char ch;
    while (file.get(ch)) {
        if (ch == '"') insideQuotes = !insideQuotes;
        else if (ch == ',' && !insideQuotes) break;
        temp += ch;
    }
}

void InsertWordByWordToTheTrie(string& text, TrieNode& trie, const string& id) {
    if (text.empty()) return;
    string word;
    istringstream iss(text);
    while (iss >> word) {
        trie.insert(word, id);
    }
}
mutex trieMutex;
/*
void SafeGetWordByWord(string& text, TrieNode& trie) {
    lock_guard<mutex> lock(trieMutex); // Protege el acceso concurrente
    InsertWordByWordToTheTrie(text, trie);
}*/


void peliculas_asignadas(vector<string> &vector_ids, unordered_map<string, pair<string, string>> & mapa){
    for (auto id: vector_ids ){
        cout << mapa[id].first << endl;
        //cout << mapa[id].second << endl;
        //cout << endl;

    }
}
int main() {
    ifstream file("C:/progra3/mpst_full_data (1).csv");
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }

    string id, title, plot_synopsis, tags, split, synopsis_source;
    int count = 0;
    TrieNode trieTitle;
    TrieNode trieSynopsis;
    unordered_map<string, pair<string,string>> mapa_ids; // ID / NOMBRE
    while (getline(file, id, ',')) {
        if (count == 0) {
            getline(file, title, ',');
            getline(file, plot_synopsis, ',');
            getline(file, tags, ',');
            getline(file, split, ',');
            getline(file, synopsis_source, '\n');
            mapa_ids[id] = {title, plot_synopsis};
            InsertWordByWordToTheTrie(title, trieTitle, id); // inserta palabra a palabra al trie, y se le asigna un id en este caso cuando sea el final de la palabra
            InsertWordByWordToTheTrie(plot_synopsis,trieSynopsis, id);
            //GetWordByWord(plot_synopsis, trieSynopsis, 2);
            count++;
            continue;
        }
        getline(file, title, ',');
        string tempSynopsis;
        GetCorrectGetLines(file, tempSynopsis); // esta función garantiza que tienen que cerrarse las comillas para que recien se pueda pasar al siguiente getline
        size_t doubleQuotePos;
        while ((doubleQuotePos = tempSynopsis.find("\"\"")) != string::npos) { // string ::npos (si es true, es porque lo encontro, y vicerversa)
            tempSynopsis.replace(doubleQuotePos, 2, "\""); // reemplaza los siguientes dos caracteres con "\""
        }
        plot_synopsis = tempSynopsis;

        string tempTags;
        GetCorrectGetLines(file, tempTags); // al ser una coma enumerativa, tiene que esperar a que termine la comilla

        tags = tempTags;
        mapa_ids[id] = {title, plot_synopsis};
        getline(file, split, ',');
        getline(file, synopsis_source, '\n');

        //thread thread1(SafeGetWordByWord, ref(title), ref(trieTitle)); // el safeword solamente sirve para que entre el thread 1

        InsertWordByWordToTheTrie(title, trieTitle,id);
        InsertWordByWordToTheTrie(plot_synopsis, trieSynopsis,id);

        //GetWordByWord(plot_synopsis, trieSynopsis, 2);
        //thread1.join();
        //trieTitle.imprimir("H");

        count++;
    }
    int option;
    cout << "1. INGRESO DE PELICULA POR NOMBRE: " << endl;
    cout << "2. INGRESO DE PELICULA POR TAG " << endl;
    cin >> option;
    if (option == 1){
        string busqueda;
        cout <<"Ingrese el nombre de la pelicula: " << endl; cin >> busqueda;
        vector<string> results = trieTitle.searchByPrefix(busqueda);
        vector<string> results2 = trieSynopsis.searchByPrefix(busqueda);
        peliculas_asignadas(results, mapa_ids);
        peliculas_asignadas(results2, mapa_ids);
    }
    else if (option == 2){
        string tag;
        cout <<"Ingrese el tag de la pelicula: " << endl; cin >> tag;

    }
    else{
        cout << "Vuelva a ingresar un numero. " << endl;
    }
    return 0;
}