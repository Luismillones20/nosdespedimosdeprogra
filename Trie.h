#ifndef PROGRA3_TRIE_H
#define PROGRA3_TRIE_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

constexpr int N = 256;
using namespace std;

class TrieNode {
private:
    unordered_set<int> movieIds; // IDs de películas donde aparece esta palabra
    string ID;
    char data; // Almacena el carácter del nodo (solo para fines de depuración o impresión)
    TrieNode* children[N];
    bool is_leaf;

    // Función auxiliar para recorrer el Trie y obtener todas las palabras completas
    void findAllWords(TrieNode* trieNode, string currentWord, vector<string>& result);

    // Función que verifica el punto de divergencia de un prefijo en el Trie
    int check_divergence(const string& word);

public:
    // Constructores
    TrieNode();
    TrieNode(char data);

    // Métodos principales
    TrieNode* insert(const string& word, const string& id);
    bool search(const string& word);
    string find_longest_prefix(const string& word);
    TrieNode* delete_word(const string& word);
    bool is_leaf_node(const string& word);

    // Búsqueda de palabras por prefijo
    void findWordsWithPrefix(const string& prefix, vector<string>& result);
    vector<string> searchByPrefix(const string& prefix);

    // Método de depuración para imprimir el Trie
    void print();
};
#endif //PROGRA3_8_TRIE_H
