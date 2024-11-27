#ifndef PROGRA3_MENU_H
#define PROGRA3_MENU_H
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "trie.h"
using namespace std;
void showMenu(
        TrieNode& trieTitle,
        TrieNode& trieSynopsis,
        TrieNode& trieTags,
        const unordered_map<string, pair<string, string>>& mapa_ids,
        chrono::duration<double> duration);
template <typename... Vectors>
void AsignedMovies(const unordered_map<string, pair<string, string>>& mapa,
                   TrieNode& trieTitle,
                   TrieNode& trieSynopsis,
                   TrieNode& trieTags,
                   chrono::duration<double> duration,
                   const Vectors&... vector_ids);
#endif //PROGRA3_MENU_H
