#include "Book.h"
#include <iostream>
using namespace std;

Book::Book()
{
    id = title = author = genre = "";
    totalCopies = 0;  // Initialize to 0
    issuedCopies = 0; // Initialize to 0
}

Book::Book(const string &_id, const string &_title, const string &_author, const string &_genre)
{
    id = _id;
    title = _title;
    author = _author;
    genre = _genre;
    totalCopies = 1; // Default is 1 copy when first created via user input
    issuedCopies = 0;
}

// New constructor to load from file (replaces the old bool constructor)
Book::Book(const string &_id, const string &_title, const string &_author, const string &_genre, int _totalCopies, int _issuedCopies)
{
    id = _id;
    title = _title;
    author = _author;
    genre = _genre;
    totalCopies = _totalCopies;
    issuedCopies = _issuedCopies;
}

void Book::printBook() const
{
    cout << "ID: " << id
         << " | Title: " << title
         << " | Author: " << author
         << " | Genre: " << genre
         << " | Total Copies: " << totalCopies
         << " | Available Copies: " << (totalCopies - issuedCopies) // Calculate available copies
         << endl;
}