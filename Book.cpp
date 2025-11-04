#include "Book.h"
#include <iostream>
using namespace std;

Book::Book() {
    id = title = author = genre = "";
    available = true;
}

Book::Book(const string& _id, const string& _title, const string& _author, const string& _genre) {
    id = _id;
    title = _title;
    author = _author;
    genre = _genre;
    available = true;
}

Book::Book(const string& _id, const string& _title, const string& _author, const string& _genre, bool _available) {
    id = _id;
    title = _title;
    author = _author;
    genre = _genre;
    available = _available;
}

void Book::printBook() const {
    cout << "ID: " << id 
         << " | Title: " << title 
         << " | Author: " << author
         << " | Genre: " << genre
         << " | Available: " << (available ? "Yes" : "No") 
         << endl;
}
