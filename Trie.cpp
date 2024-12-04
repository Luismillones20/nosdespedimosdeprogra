#include "Trie.h"

#include <future>
#include <iostream>
#include <sstream>

// Constructor por defecto
TrieNode::TrieNode() : is_leaf(false), data('\0') { // Constructor por defecto
    for (int i = 0; i < N; i++) {                   // Se inicializan los 256 hijos (cant de caracteres ASCII)
        children[i] = nullptr;                      // A cada uno se asigna un nullptr (no apuntan a nada aún)
    }
}

// Constructor que acepta un carácter de datos
TrieNode::TrieNode(char data) : data(data), is_leaf(false) {   // Constructor con un solo atributo
    for (int i = 0; i < N; i++) {               // Se inicializan los 256 hijos (cant de caracteres ASCII)
        children[i] = nullptr;                  // A cada uno se asigna un nullptr (no apuntan a nada aún)
    }
}

// Inserta una palabra en el Trie, asociándola con un ID específico
TrieNode* TrieNode::insert(const string& word, const string& id) {
    TrieNode* temp = this;                                  // temp es este árbol donde se está buscando
    for (char ch : word) {                                  // Recorre cada caracter de la palabra
        int idx = static_cast<unsigned char>(ch);           // Pasa la letra a codigo ascii
        if (temp->children[idx] == nullptr) {               // Busca la letra entre los hijos de ese arbol
            temp->children[idx] = new TrieNode(ch);    // Si no lo encuentra, crea un nuevo surbarbol con esa letra
        }
        temp = temp->children[idx]; // Ahora el arbol donde se buscará será en ese subarbol con la letra buscada
    }
    // Cuando se terminan de recorrer todas las letras, se indica que estamos en una hoja (arbol/nodo sin hijos)
    temp->is_leaf = true;

    // El id de este arbol sin hijos será el id de la palabra de la pelicula que se ingresó
    // El problema es que si se ingresa una misma palabra que antes, el id de esa palabra cambiará al id de la
    // película más reciente que tenga esa palabra también. No se guardan todos los ids, solo el de la ultima pelic.
    // Por eso, al buscar "Shrek", solo sale "Shrek 2", el cual está más al final en el csv
    // Y esto se visualiza al buscar palabras tanto en peliculas/sinopsis como en tags
    temp->ID = id;

    // Devuelve el arbol original
    return this;
}

// Busca una palabra en el Trie
bool TrieNode::search(const string& word) {
    TrieNode* temp = this;
    for (char ch : word) {
        int idx = static_cast<unsigned char>(ch);
        if (temp->children[idx] == nullptr) {
            return false;
        }
        temp = temp->children[idx];
    }
    return temp->is_leaf;
}

// Encuentra el prefijo más largo que no diverge en el Trie
string TrieNode::find_longest_prefix(const string& word) {
    if (word.empty()) return "";

    string longest_prefix = word;
    int branch_idx = check_divergence(longest_prefix) - 1;
    if (branch_idx >= 0) {
        longest_prefix = longest_prefix.substr(0, branch_idx);
    }
    return longest_prefix;
}

// Función auxiliar para obtener el punto de divergencia en un prefijo
int TrieNode::check_divergence(const string& word) {
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

// Borra una palabra del Trie
TrieNode* TrieNode::delete_word(const string& word) {
    if (!this) return nullptr;
    if (word.empty()) return this;
    if (!is_leaf_node(word)) return this;

    TrieNode* temp = this;
    string longest_prefix = find_longest_prefix(word);

    if (longest_prefix.empty()) {
        return this;
    }

    for (size_t i = 0; i < longest_prefix.size(); ++i) {
        int position = static_cast<unsigned char>(longest_prefix[i]);
        if (temp->children[position] != nullptr) {
            temp = temp->children[position];
        } else {
            return this;
        }
    }

    for (size_t i = longest_prefix.size(); i < word.size(); ++i) {
        int position = static_cast<unsigned char>(word[i]);
        if (temp->children[position]) {
            TrieNode* rm_node = temp->children[position];
            temp->children[position] = nullptr;
            delete rm_node;
        }
    }
    return this;
}

// Verifica si una palabra termina en un nodo hoja
bool TrieNode::is_leaf_node(const string& word) {
    TrieNode* temp = this;
    for (char c : word) {
        int position = static_cast<unsigned char>(c);
        if (temp->children[position]) {
            temp = temp->children[position];
        } else {
            return false;
        }
    }
    return temp->is_leaf;
}

// Encuentra todas las palabras que inician con un prefijo dado
void TrieNode::findWordsWithPrefix(const string& prefix, vector<string>& result) { // prefix y recommendations
    TrieNode* temp = this;                          // temp es este árbol donde se está buscando
    for (char c : prefix) {                         // Recorre letra por letra en la palabra
        int idx = static_cast<unsigned char>(c);    // Pasa la letra a codigo ascii
        if (temp->children[idx] == nullptr) return; // Busca la letra en el hijo de ese arbol que sea esa letra
        // Si no encuentra, solo se regresa (recommendations no se actualiza)
        temp = temp->children[idx];     // Ahora el árbol donde se buscará es el subarbol donde estaba la letra buscada
    }
    findAllWords(temp, prefix, result); // Ahora, se manda el subarbol con la palabra,
    // la palabra y el vector de recommendations
}

// Función auxiliar para encontrar todas las palabras a partir de un nodo dado
void TrieNode::findAllWords(TrieNode* trieNode, string currentWord, vector<string>& result) {
    if (trieNode == nullptr) return;    // Si ya no se encuentran mas hijos (normalmente cuando el for de abajo no
    // encontró ningún subarbol)

    if (trieNode->is_leaf) {            // Si la palabra está completa,
        result.push_back(trieNode->ID); // se agrega a los resultados el Id de pelicula que contiene esa palabra
    }

    for (int i = 0; i < N; i++) {
        if (trieNode->children[i] != nullptr) {     // Si existe el hijo de ese caracter ASCII, entra al condicional
            char nextChar = static_cast<char>(i);   // Esa letra-hija que sí existe se pasa a caracter ASCII
            findAllWords(trieNode->children[i], currentWord + nextChar, result);
            // y ahora se buscarán en el subarbol de esa letra-hija, palabras que sean palabraActual + esa letra-hija
        }
    }
}

// Busca palabras usando un prefijo
vector<string> TrieNode::searchByPrefix(const string& prefix) {
    // el prefix es la palabra (entera o a medias) a buscar
    vector<string> resultsIds;                  // Vector con todas las recomendaciones guardadas
    findWordsWithPrefix(prefix, resultsIds); // Se actualiza aca ese vector
    return resultsIds;                          // Retorna todas las recomendaciones
}

// Imprime el Trie (para depuración)
void TrieNode::print() {
    if (this) {
        cout << this->data << " -> ";
        for (int i = 0; i < N; i++) {
            if (children[i] != nullptr) {
                children[i]->print();
            }
        }
    }
}

void TrieNode::parallelFindWordsWithPrefix(const vector<string>& prefixes, vector<vector<string>>& results) {
    vector<future<void>> futures;
    for (size_t i = 0; i < prefixes.size(); ++i) {
        futures.push_back(std::async(std::launch::async, [this, &prefixes, &results, i]() {
            findWordsWithPrefix(prefixes[i], results[i]);
        }));
    }
    for (auto& f : futures) {
        f.get(); // Asegurarse de que todos los hilos terminen antes de continuar
    }
}
