#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
using namespace std;

class Book
{
public:
    string id;
    string title;
    string author;
    string genre;
    // --- NEW: Replaced 'bool available' with copy counters ---
    int totalCopies = 0;  // Total number of copies of this title
    int issuedCopies = 0; // Number of copies currently issued

    Book();
    // Constructor for new book entry (default 1 copy)
    Book(const string &_id, const string &_title, const string &_author, const string &_genre);
    // New constructor for loading from file
    Book(const string &_id, const string &_title, const string &_author, const string &_genre, int _totalCopies, int _issuedCopies);

    void printBook() const;
};

#endif // BOOK_H