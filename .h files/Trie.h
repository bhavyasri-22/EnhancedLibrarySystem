#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <memory>
using namespace std;

// Simple Trie that stores titles (full strings) and supports prefix search.
// Returns up to a limit of matching titles.
class Trie {
private:
    struct Node {
        bool end = false;
        vector<shared_ptr<Node>> next;
        // store children by lowercase letter and other chars -> using 128 ascii bucket
        string word; // store the full word at terminal nodes
        Node(): next(128, nullptr), end(false), word("") {}
    };

    shared_ptr<Node> root;
    void collect(shared_ptr<Node> node, vector<string>& out, int limit);

public:
    Trie();
    void insert(const string& word);
    vector<string> searchPrefix(const string& prefix, int limit = 10);
    void remove(const string& word);
    void clear();
};

#endif // TRIE_H
