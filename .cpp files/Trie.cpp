#include "Trie.h"
#include <algorithm>
#include <cctype>
#include <queue>

static inline int idx_of_char(char c) {
    // limit to ASCII 0..127
    unsigned char uc = static_cast<unsigned char>(c);
    return (int)uc;
}

Trie::Trie() {
    root = make_shared<Node>();
}

void Trie::insert(const string& word) {
    auto node = root;
    for(char c : word) {
        int i = idx_of_char(c);
        if(!node->next[i]) node->next[i] = make_shared<Node>();
        node = node->next[i];
    }
    node->end = true;
    node->word = word;
}

vector<string> Trie::searchPrefix(const string& prefix, int limit) {
    vector<string> results;
    auto node = root;
    for(char c : prefix) {
        int i = idx_of_char(c);
        if(!node->next[i]) return results;
        node = node->next[i];
    }
    // BFS/DFS from node to collect words
    collect(node, results, limit);
    return results;
}

void Trie::collect(shared_ptr<Node> node, vector<string>& out, int limit) {
    if(!node) return;
    if(node->end) {
        out.push_back(node->word);
        if((int)out.size() >= limit) return;
    }
    for(int i = 0; i < (int)node->next.size(); ++i) {
        if(node->next[i]) {
            collect(node->next[i], out, limit);
            if((int)out.size() >= limit) return;
        }
    }
}

void Trie::remove(const string& word) {
    // naive removal: mark end=false if found. Does not free nodes.
    auto node = root;
    for(char c : word) {
        int i = idx_of_char(c);
        if(!node->next[i]) return;
        node = node->next[i];
    }
    if(node->end) {
        node->end = false;
        node->word = "";
    }
}

void Trie::clear() {
    root = make_shared<Node>();
}
