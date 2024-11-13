#include "trie.h"
#include <iostream>
#include <sstream>

// Constructor por defecto
TrieNode::TrieNode() : is_leaf(false), data('\0') {
    for (int i = 0; i < N; i++) {
        children[i] = nullptr;
    }
}

// Constructor que acepta un carácter de datos
TrieNode::TrieNode(char data) : data(data), is_leaf(false) {
    for (int i = 0; i < N; i++) {
        children[i] = nullptr;
    }
}

// Inserta una palabra en el Trie, asociándola con un ID específico
TrieNode* TrieNode::insert(const string& word, const string& id) {
    TrieNode* temp = this;
    for (char ch : word) {
        int idx = static_cast<unsigned char>(ch);
        if (temp->children[idx] == nullptr) {
            temp->children[idx] = new TrieNode(ch);
        }
        temp = temp->children[idx];
    }
    temp->is_leaf = true;
    temp->ID = id;
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
void TrieNode::findWordsWithPrefix(const string& prefix, vector<string>& result) {
    TrieNode* temp = this;
    for (char c : prefix) {
        int idx = static_cast<unsigned char>(c);
        if (temp->children[idx] == nullptr) return;
        temp = temp->children[idx];
    }
    findAllWords(temp, prefix, result);
}

// Función auxiliar para encontrar todas las palabras a partir de un nodo dado
void TrieNode::findAllWords(TrieNode* trieNode, string currentWord, vector<string>& result) {
    if (trieNode == nullptr) return;

    if (trieNode->is_leaf) {
        result.push_back(trieNode->ID);
    }

    for (int i = 0; i < N; i++) {
        if (trieNode->children[i] != nullptr) {
            char nextChar = static_cast<char>(i);
            findAllWords(trieNode->children[i], currentWord + nextChar, result);
        }
    }
}

// Busca palabras usando un prefijo
vector<string> TrieNode::searchByPrefix(const string& prefix) {
    vector<string> recommendations;
    findWordsWithPrefix(prefix, recommendations);
    return recommendations;
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
