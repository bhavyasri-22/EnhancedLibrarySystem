#include "Trie.h"
Trie::Trie() { root = new TrieNode(); }

void Trie::insert(const string &word) {
    TrieNode* node = root;
    for(char c : word) {
        if(!node->children[c]) node->children[c] = new TrieNode();
        node = node->children[c];
    }
    node->isEnd = true;
}

void Trie::dfs(TrieNode* node, string prefix, vector<string>& results) {
    if(node->isEnd) results.push_back(prefix);
    for(auto &p : node->children) dfs(p.second, prefix + p.first, results);
}

vector<string> Trie::searchPrefix(const string &prefix) {
    TrieNode* node = root;
    vector<string> results;
    for(char c : prefix) {
        if(!node->children[c]) return results;
        node = node->children[c];
    }
    dfs(node, prefix, results);
    return results;
}
