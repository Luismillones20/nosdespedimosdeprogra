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
#include "Trie.h"
#include <algorithm>
#include "menu.h"
#include <Archivo.h>

#include "Movie.h"

//constexpr int N = 256;
using namespace std;

// TAREAS:
// 2. Que tenga una funcion thread que recorra una parte y otro otra
// 3. Ponerle un sistema de puntuación en el cual se elija que pelicula debe ir primero
// 4. Sistema de likes (recomendar peliculas similiares), igual con ver mas tarde
// 5. Usar el singleton, iterator, memento, guardar las busquedas
// 6. Busqueda por varias palabras no solo ingresar strings sino bloques
// 7. Imprimir 5 peliculas en orden, y luego un boton para las siguientes 5 peliculas // FUNCION REALIZADA
// 8. Busqueda por tag // FUNCION REALIZADA
// 9. Arreglar el tema de que no recorre ciertas palabras
// 10. Ahorita nomas está buscando en titulo, también debe buscar en sinopsis // FUNCION REALIZADA

// Primer commit de sebastian :D

void GetCorrectGetLines(ifstream  &file, string &temp) {
    bool insideQuotes = false;
    char ch;
    while (file.get(ch)) {
        if (ch == '"') insideQuotes = !insideQuotes; // Lee un " y lo cambia de true a false o viceversa.
            // En caso haya "", ocurre: true->false->true (no queda en false)
        else if (ch == ',' && !insideQuotes) break; // Si encuentra una coma y ya no se está dentro de las comillas,
            // entonces detiene el recorrido de caracteres.
        else temp += ch;
    }
}

void GetCorrectTitleAndTag(ifstream& file, string& tempTags){
    bool insideQuotes = false;
    bool thereAreQuotationMarks = false;
    char ch;
    while (file.get(ch)) {
        if (ch == '"'){
            insideQuotes = !insideQuotes; // Lee un " y lo cambia de true a false o viceversa.
            thereAreQuotationMarks = true;
        }
            // En caso haya "", ocurre: true->false->true (no queda en false)
        else if (ch == ',' && !insideQuotes) break; // Si encuentra una coma y ya no se está dentro de las comillas,
            // entonces detiene el recorrido de caracteres.

        else if (ch == ',' && !thereAreQuotationMarks) break; // En caso haya un solo tag, en el momento
            // que se lee la primera coma, se deja de añadir los caracteres

        else{
            if(ch != ',') tempTags += ch;
        }
    }

    //Lo que lee y manda esto al final es un string completo:
    // ej: En caso haya un solo tag, envía -> horror
    // ej: En caso haya varios tags, envía -> horror, action, dark, violence (todo como un solo string)

}

void InsertWordByWordToTheTrie(string& text, TrieNode& trie, const string& id) {
    if (text.empty()) return; // Si no hay nada en el texto, se termina la función
    string word;
    istringstream iss(text);
    while (iss >> word) {
        trie.insert(word, id); // Método de ese árbol (de Titles, Synopsis o Tags)
        // Se va pasando palabra por palabra para insertar cada una en el arbol
    }
}
mutex trieMutex;
void SafeInsertWordByWordToTheTrie(string& text, TrieNode& trie, const string& id) {
    lock_guard<mutex> lock(trieMutex); // Lock for thread-safety
    InsertWordByWordToTheTrie(text, trie, id);
}
// Use this function to handle concurrent insertion into the Trie
void InsertDataConcurrently(vector<string> &texts, TrieNode &trie, vector<string> &ids) {
    vector<thread> threads;
    for (size_t i = 0; i < texts.size(); ++i) {
        threads.emplace_back(SafeInsertWordByWordToTheTrie, ref(texts[i]), ref(trie), ref(ids[i]));
    }
    for (auto &t : threads) {
        t.join(); // Ensure all threads complete
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    Archivo& archivo = Archivo::getArchivo("../mpst_full_data (1).csv");
    ifstream& file = archivo.getFile();

    string id, title, plot_synopsis, tags, split, synopsis_source;
    int count = 0;
    TrieNode trieTitle;
    TrieNode trieSynopsis;
    unordered_map<string, Movie*> mapa_ids; // ID-> (TITULO, SINOPSIS)
    TrieNode trieTags;
    auto start = chrono::high_resolution_clock::now();

    string primeraLinea;
    getline(file, primeraLinea); // Se lee la linea imdb_id,title,plot_synopsis,tags,split,synopsis_source
    while (getline(file, id, ',')) { // Siempre que se pueda leer un id, sigue corriendo el programa
        string tempTitle;
        GetCorrectTitleAndTag(file, tempTitle); // Función para obtener bien el titulo de la pelicula
        title = tempTitle;

        string tempSynopsis;
        GetCorrectGetLines(file, tempSynopsis); // esta función garantiza que tienen que cerrarse las comillas para que recien se pueda pasar al siguiente getline
        size_t doubleQuotePos;
        while ((doubleQuotePos = tempSynopsis.find("\"\"")) != string::npos) { // string ::npos (si es true, es porque lo encontró, y viceversa)
            tempSynopsis.replace(doubleQuotePos, 2, "\""); // reemplaza los siguientes dos caracteres con "\""
        }
        plot_synopsis = tempSynopsis;

        string tempTags;
        GetCorrectTitleAndTag(file, tempTags);(file, tempTags); // al ser una coma enumerativa, tiene que esperar a que termine la comilla
        tags = tempTags;

        mapa_ids[id] = new Movie(id,title, plot_synopsis); // Se crea una peli con el id leido, nombre y synopsis
        getline(file, split, ','); // Se obtiene el "split" y no se guarda
        getline(file, synopsis_source, '\n'); // Se obtiene el "synopsis_source" y no se guarda

        SafeInsertWordByWordToTheTrie(title, trieTitle, id);
        SafeInsertWordByWordToTheTrie(plot_synopsis, trieSynopsis, id);
        SafeInsertWordByWordToTheTrie(tags, trieTags, id);

        count++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    showMenu(trieTitle, trieSynopsis, trieTags, mapa_ids, duration);

    return 0;
} // comentarioxd