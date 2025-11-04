#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEnd = false;
};

class Trie {
private:
    TrieNode* root;
    void dfs(TrieNode* node, string prefix, vector<string>& results);

public:
    Trie();
    void insert(const string &word);
    vector<string> searchPrefix(const string &prefix);
};

#endif



// #ifndef TRIE_H
// #define TRIE_H

// #include <unordered_map>
// #include <vector>
// #include <string>
// using namespace std;

// class TrieNode {
// public:
//     unordered_map<char, TrieNode*> children;
//     bool isEnd;
//     vector<string> titles;

//     TrieNode() : isEnd(false) {}
// };

// class Trie {
// private:
//     TrieNode* root;

// public:
//     Trie() { root = new TrieNode(); }

//     void insert(const string &word) {
//         TrieNode* node = root;
//         for(char c : word) {
//             if(!node->children[c]) node->children[c] = new TrieNode();
//             node = node->children[c];
//             node->titles.push_back(word);
//         }
//         node->isEnd = true;
//     }

//     vector<string> autocomplete(const string &prefix) {
//         TrieNode* node = root;
//         for(char c : prefix) {
//             if(!node->children[c]) return {};
//             node = node->children[c];
//         }
//         return node->titles;
//     }
// };

// #endif
