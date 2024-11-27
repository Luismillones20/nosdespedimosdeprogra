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
#include <algorithm>
#include "menu.h"
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




// SOLUCIONES EN MENTE:


// 2
// 7. El like tendria un peso de 5, el ver mas tarde un peso de 3, se compararía cuales tienen mayor cantidad y se pondrían, basicamente un if si es que tiene un ver mas tarde o un like se pondría al inicio
// 5. En el caso de singleton se pondría a la base de datos
//      En el caso de memento se pondría para recordar las busquedas que ha realizado
// 6. El más sencillo hasta ahora, poner q en lugar de un string reciba varios.
// 10.
// 8. En la busqueda por tag se tendria que comparar si es que está el tag se podria poner tuplas en lugar de pairs, en este caso if tupla pertenece a peliculas se pone la pelicula,
// 9. CREO QUE EL PROBLEMA ES EL HECHO DE Q ALGUNOS CARACTERES LOS CONSIDERA COMO PALABRAS, POR EJEMPLO ARTHUR:


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
    ifstream file("C:/progra3/mpst_full_data (1).csv");
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }

    string id, title, plot_synopsis, tags, split, synopsis_source;
    int count = 0;
    TrieNode trieTitle;
    TrieNode trieSynopsis;
    unordered_map<string, pair<string,string>> mapa_ids; // ID-> (TITULO, SINOPSIS)
    TrieNode trieTags;
    auto start = chrono::high_resolution_clock::now();
    while (getline(file, id, ',')) {
        if (count == 0) {
            getline(file, title, ',');
            getline(file, plot_synopsis, ',');
            getline(file, tags, ',');
            getline(file, split, ',');
            getline(file, synopsis_source, '\n');
            mapa_ids[id] = {title, plot_synopsis};
            //InsertWordByWordToTheTrie(title, trieTitle, id); // inserta palabra a palabra al trie, y se le asigna un id en este caso cuando sea el final de la palabra
            //InsertWordByWordToTheTrie(plot_synopsis,trieSynopsis, id);
            //InsertWordByWordToTheTrie(tags,trieTags, id);
            //GetWordByWord(plot_synopsis, trieSynopsis, 2);
            SafeInsertWordByWordToTheTrie(title, trieTitle, id);
            SafeInsertWordByWordToTheTrie(plot_synopsis, trieSynopsis, id);
            SafeInsertWordByWordToTheTrie(tags, trieTags, id);

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
        // FUNCIONES IMPORTANTES:
        //InsertWordByWordToTheTrie(title, trieTitle,id);
        //InsertWordByWordToTheTrie(plot_synopsis, trieSynopsis,id);
        // InsertWordByWordToTheTrie(tags,trieTags, id);
        SafeInsertWordByWordToTheTrie(title, trieTitle, id);
        SafeInsertWordByWordToTheTrie(plot_synopsis, trieSynopsis, id);
        SafeInsertWordByWordToTheTrie(tags, trieTags, id);

        //GetWordByWord(plot_synopsis, trieSynopsis, 2);
        //thread1.join();
        count++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    showMenu(trieTitle, trieSynopsis, trieTags, mapa_ids, duration);
    return 0;
}
// FALTA ARREGLAR LA FUNCION DE QUE RECIBA VARIOS A LA VEZ// FALTA ARREGLAR LA FUNCION DE QUE RECIBA VARIOS A LA VEZ