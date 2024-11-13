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
constexpr int N = 256;
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

class TrieNode {
    unordered_set<int> movieIds; // IDs de películas donde aparece esta palabra
    string ID;
    char data; // Storing for printing purposes only
    TrieNode* children[N];
    bool is_leaf;
public:
    // constructor de TrieNode, la hoja es falsa por defecto y le crea
    // un array de 256 caracteres todos con un puntero nullptr
    TrieNode(){
        for (int i = 0; i < N; i++) {
            children[i] = nullptr;
        }
    }
    TrieNode(char data) {
        this->data = data;
        this->is_leaf = false;
        for (int i = 0; i < N; i++) {
            children[i] = nullptr;
        }
    }

    // funciones:

    // insertar un nuevo nodo, si es que es un nullptr lo crea
    // si no lo es, ahora el nuevo temp, es ahora el children[i]
    // cuando llega al final is_leaf se vuelve true y retorna actualizado

    TrieNode* insert(string word, string id) {
        TrieNode* temp = this; // temp = trienode que se está usando
        for (int i = 0; word[i] != '\0'; i++) {
            int idx = static_cast<unsigned char>(word[i]);
            if (temp->children[idx] == nullptr) {
                temp->children[idx] = new TrieNode(word[i]);
            }
            temp = temp->children[idx];
        }
        temp->is_leaf = true;
        temp->ID = id; // el id se agrega al final de la palabra
        return this;
    }

    // Nuevamente tmp se vuelve un puntero temporal, si es que no lo encuentra devuelve false
    // si es que lo encuentra se reemplaza, hasta cuando llegue al final, si es que
    // no ha retornado false hasta ahora, entonces retornará un true

    bool search(string word) {
        TrieNode* temp = this;
        for (int i = 0; word[i] != '\0'; i++) {
            int idx = (unsigned char) word[i];
            if (temp->children[idx] == nullptr) {
                return false;
            }
            temp = temp->children[idx];
        }
        return temp->is_leaf;
    }

    //Nuevamente se usa un temp
    // Recorre la longitud de la palabra, Si el caracter no es null
    int check_divergence(string word) {
        TrieNode* temp = this;
        int len = word.length();
        if (len == 0) return 0;
        int last_index = 0;

        for (int i = 0; i < len; i++) {
            int position = static_cast<unsigned char>(word[i]);
            if (temp->children[position]) {
                for (int j = 0; j < N; j++) {
                    if (j != position && temp->children[j]) {
                        last_index = i + 1;
                        break;
                    }
                }
                temp = temp->children[position];
            }
        }
        return last_index;
    }
    string find_longest_prefix(string word) {
        if (word.empty()) return "";

        string longest_prefix = word;
        int branch_idx = check_divergence(longest_prefix) - 1;
        if (branch_idx >= 0) {
            longest_prefix = longest_prefix.substr(0, branch_idx);
        }
        return longest_prefix;
    }
    TrieNode* delete_word(const std::string& word) {
        if (!this) return nullptr;
        if (word.empty()) return this;
        if (!is_leaf_node(word)) return this;

        TrieNode* temp = this;
        string longest_prefix = find_longest_prefix(word);

        if (longest_prefix.empty()) { // Si el prefijo más largo está vacío
            return this;
        }

        // Recorrer el prefijo más largo y navegar por el árbol Trie
        for (size_t i = 0; i < longest_prefix.size(); ++i) {
            int position = static_cast<unsigned char>(longest_prefix[i]);
            if (temp->children[position] != nullptr) {
                temp = temp->children[position];
            } else {
                return this; // Si no se encuentra el nodo en el prefijo más largo, no borrar nada
            }
        }

        // Ahora borrar las palabras restantes que no formen parte del prefijo
        size_t len = word.size();
        for (size_t i = longest_prefix.size(); i < len; ++i) {
            int position = static_cast<unsigned char>(word[i]);
            if (temp->children[position]) {
                TrieNode* rm_node = temp->children[position];
                temp->children[position] = nullptr;
                delete rm_node; // Eliminar el nodo
            }
        }
        return this;
    }

    bool is_leaf_node(const std::string& word) {
        TrieNode* temp = this;
        for (char c : word) {
            int position = static_cast<unsigned char>(c); // Obtener el valor de la posición
            if (temp->children[position]) {
                temp = temp->children[position]; // Avanzar en el árbol Trie
            } else {
                return false; // Si no se encuentra el nodo, no es una hoja
            }
        }
        return temp->is_leaf; // Retornar si es una hoja al final del recorrido
    }


    // Print the Trie (for debugging purposes)
    void print() {
        if (this) {
            cout << this->data << " -> ";
            for (int i = 0; i < N; i++) {
                if (children[i] != nullptr) {
                    children[i]->print();
                }
            }
        }
    }

    void findWordsWithPrefix(const string& prefix, vector<string>& result) {
        TrieNode* temp = this;

        // Buscar el nodo final del prefijo
        for (char c : prefix) {
            int idx = static_cast<unsigned char>(c);
            if (temp->children[idx] == nullptr) return;  // No hay palabras con este prefijo
            temp = temp->children[idx];
        }

        // Iniciar la búsqueda de todas las palabras desde el nodo del prefijo
        findAllWords(temp, prefix, result);
    }
    // Función auxiliar para recorrer el Trie y obtener todas las palabras completas
    void findAllWords(TrieNode* trieNode, string currentWord, vector<string>& result) {
        if (trieNode == nullptr) return;

        // Si es un nodo final de palabra, agregar la palabra actual a los resultados
        if (trieNode->is_leaf) {
            result.push_back(trieNode->ID);
        }

        // Recorrer todos los hijos para obtener más palabras
        for (int i = 0; i < N; i++) {
            if (trieNode->children[i] != nullptr) {
                char nextChar = static_cast<char>(i);
                findAllWords(trieNode->children[i], currentWord + nextChar, result);
            }
        }
    }

    vector<string> searchByPrefix(const string& prefix) {
        vector<string> recommendations;
        findWordsWithPrefix (prefix, recommendations);
        return recommendations;
    }
    /*
    ~TrieNode() {
        for (auto& child : childrenTitle) {
            delete child.second;  // Elimina cada nodo hijoggggg
        }
    }*/
};

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
        cout << mapa[id].second << endl;
        cout << endl;

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
        //GetWordByWord(plot_synopsis, trieSynopsis, 2);
        //thread1.join();
        //trieTitle.imprimir("H");

        count++;
    }
    string busqueda;
    cout <<"Ingrese el nombre de la pelicula: " << endl; cin >> busqueda;
    vector<string> results = trieTitle.searchByPrefix(busqueda);
    //cout << trieTitle.search("Mr");
    peliculas_asignadas(results, mapa_ids);
    return 0;
}