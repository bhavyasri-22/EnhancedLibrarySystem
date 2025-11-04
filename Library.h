#ifndef LIBRARY_H
#define LIBRARY_H

#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include "Book.h"
#include "Student.h"
#include "Trie.h"
using namespace std;

enum OperationType { ADD_BOOK, REMOVE_BOOK, ISSUE_BOOK, RETURN_BOOK };

struct Operation {
    OperationType type;
    string bookID;
    string studentID;
    Book bookBackup;
};

class Library {
private:
    unordered_map<string, Book> books;
    unordered_map<string, Student> students;
    Trie bookTrie;
    queue<pair<string,string>> waitingList;
    stack<Operation> undoStack;
    stack<Operation> redoStack;

public:
    Library();
    ~Library();
    void addBook(const Book &b);
    void removeBook(const string &bookID);
    void searchBookByTitle(const string &title);
    void addStudent(const Student &s);
    void issueBook(const string &bookID, const string &studentID);
    void returnBook(const string &bookID, const string &studentID);
    vector<string> autocompleteTitle(const string &prefix);
    void undoLastOperation();
    void redoLastOperation();
    void displayAllBooks();
    void displayIssuedBooks();
    void saveToFile();
    void loadFromFile();
};

#endif
