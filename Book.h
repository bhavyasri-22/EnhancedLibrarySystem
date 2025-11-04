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
    bool available;

    Book();
    Book(string _id, string _title, string _author);
    Book(string _id, string _title, string _author, bool _available);

    void printBook() const;
};

#endif
