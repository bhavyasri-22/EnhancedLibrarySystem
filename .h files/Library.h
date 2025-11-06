#ifndef LIBRARY_H
#define LIBRARY_H

#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <string>
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
    unordered_map<string, Book> books;      // key = book.id
    unordered_map<string, Student> students;// key = student.id
    Trie bookTrie;                          // stores book titles (case-insensitive)
    queue<pair<string,string>> waitingList; // pair<bookID, studentID>
    stack<Operation> undoStack;
    stack<Operation> redoStack;

    void performIssueWithoutRecord(const string& bookID, const string& studentID);
    void performReturnWithoutRecord(const string& bookID, const string& studentID);

public:
    Library();
    ~Library();

    void addBook(const Book &b);
    void removeBook(const string &bookID);

    void searchBookByTitle(const string &title);
    void searchBookByID(const string &bookID);

    void addStudent(const Student &s);
    void issueBook(const string &bookID, const string &studentID);
    void returnBook(const string &bookID, const string &studentID);

    vector<string> autocompleteTitle(const string &prefix);

    void displayBooksByGenre(const string &genre);
    vector<string> availableGenres() const;

    void displayAllBooks();
    void displayIssuedBooks();

    void undoLastOperation();
    void redoLastOperation();

    void saveToFile();
    void loadFromFile();
};

#endif // LIBRARY_H
