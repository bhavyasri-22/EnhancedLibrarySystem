#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
using namespace std;

class Book {
public:
    string id;
    string title;
    string author;
    string genre;
    bool available;

    Book();
    Book(const string& _id, const string& _title, const string& _author, const string& _genre);
    Book(const string& _id, const string& _title, const string& _author, const string& _genre, bool _available);

    void printBook() const;
};

#endif // BOOK_H
