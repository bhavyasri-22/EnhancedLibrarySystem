#include "Book.h"
Book::Book() : id(""), title(""), author(""), available(true) {}
Book::Book(string _id, string _title, string _author) : id(_id), title(_title), author(_author), available(true) {}
Book::Book(string _id, string _title, string _author, bool _available) : id(_id), title(_title), author(_author), available(_available) {}

void Book::printBook() const {
    cout << "ID: " << id << " | Title: " << title << " | Author: " << author 
         << " | Available: " << (available ? "Yes" : "No") << "\n";
}